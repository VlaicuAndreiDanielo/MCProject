#include "GameManager.h"
#include "LobbyManager.h"
#include <chrono>
#include <iostream>

extern LobbyManager lobbyManager;  //TODO check if I should make lobbyManager and gameManager singletons.

GameManager::GameManager() : m_nextGameId(GameConfig::kfirstGameId) {}

GameManager::~GameManager() {
    for (auto& [gameId, _] : m_games) {
        StopGameLoop(gameId);
    }
    for (auto& [gameId, gameState] : m_games) {
        delete gameState;
    }
}

int GameManager::CreateGameFromLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(m_gameMutex);

    int gameId = m_nextGameId++;

    Lobby* lobby = lobbyManager.GetLobby(lobbyId);
    if (!lobby) {
        throw std::runtime_error("Lobby not found");
    }

    // Create game state and add all lobby players
    GameState* gameState = new GameState();
    const auto& playersMap = lobby->GetPlayers();
    for (const auto& [playerId, isReady] : playersMap) {
        gameState->AddPlayer(playerId);
    }

    m_games[gameId] = gameState;
    m_runningGames[gameId] = false;

    return gameId;
}

void GameManager::DeleteGame(int gameId) {
    StopGameLoop(gameId);

    std::lock_guard<std::mutex> lock(m_gameMutex);
    if (m_games.find(gameId) != m_games.end()) {
        delete m_games[gameId];
        m_games.erase(gameId);
        m_gameThreads.erase(gameId);
        m_runningGames.erase(gameId);
    }
}

bool GameManager::StartGameLoop(int gameId) {
    std::lock_guard<std::mutex> lock(m_gameMutex);

    if (m_runningGames[gameId]) {
        return true;
    }

    m_runningGames[gameId] = true;

    try {
        m_gameThreads[gameId] = std::thread(&GameManager::GameLoop, this, gameId);
        return true;  // Successfully started the game loop
    }
    catch (const std::exception& e) {
        m_runningGames[gameId] = false;  // Reset the state if starting the thread fails
        return false;  // Indicate failure
    }
}

void GameManager::StopGameLoop(int gameId) {
    std::lock_guard<std::mutex> lock(m_gameMutex);

    if (!m_runningGames[gameId]) {
        return;
    }

    m_runningGames[gameId] = false;
    if (m_gameThreads[gameId].joinable()) {
        m_gameThreads[gameId].join();
    }
    m_gameThreads.erase(gameId);
}

float GameManager::GetDeltaTime()
{
    return deltaTime;
}

GameState* GameManager::GetGameState(int gameId) {
    std::lock_guard<std::mutex> lock(m_gameMutex);

    if (m_games.find(gameId) != m_games.end()) {
        return m_games[gameId];
    }
    return nullptr;
}

void GameManager::GameLoop(int gameId) {
    const std::chrono::milliseconds frameDuration(GameConfig::kFrameDurationMs);
    auto previousTime = std::chrono::steady_clock::now();
    std::chrono::nanoseconds totalTime = std::chrono::nanoseconds::zero();

    while (m_runningGames[gameId]) {
        auto currentTime = std::chrono::steady_clock::now();

        // Accumulate elapsed time
        totalTime += (currentTime - previousTime);
        previousTime = currentTime;

        // Check if enough time has passed for a frame
        if (totalTime >= std::chrono::duration_cast<std::chrono::nanoseconds>(frameDuration)) {
            // Calculate deltaTime in seconds
            deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(totalTime).count();

            // Update the game state with deltaTime
            {
                std::lock_guard<std::mutex> lock(m_gameMutex);
                if (m_games.find(gameId) != m_games.end()) {
                    m_games[gameId]->UpdateGame(deltaTime);
                }
                else {
                    break;
                }
            }

            // Reset totalTime after processing the frame
            totalTime = std::chrono::nanoseconds::zero();
        }
    }
}