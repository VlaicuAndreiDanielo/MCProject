#include "GameManager.h"
#include "LobbyManager.h"
#include <chrono>
#include <iostream>

extern LobbyManager lobbyManager;

GameManager::GameManager() {}

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

    static int nextGameId = 1;
    int gameId = nextGameId++;

    Lobby* lobby = lobbyManager.GetLobby(lobbyId);
    if (!lobby) {
        throw std::runtime_error("Lobby not found");
    }

    // Create game state and add all lobby players
    GameState* gameState = new GameState();
    const auto& playersMap = lobby->GetPlayers(); // Access players and their ready statuses
    for (const auto& [playerId, isReady] : playersMap) {
        gameState->AddPlayer(playerId); // Add each player to the game
    }

    m_games[gameId] = gameState;
    m_runningGames[gameId] = false;

    return gameId;
}

void GameManager::deleteGame(int gameId) {
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
        return true;  // Game loop is already running
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

GameState* GameManager::GetGameState(int gameId) {
    std::lock_guard<std::mutex> lock(m_gameMutex);

    if (m_games.find(gameId) != m_games.end()) {
        return m_games[gameId];
    }
    return nullptr;
}

void GameManager::GameLoop(int gameId) {
    const std::chrono::milliseconds frameDuration(16); // ~60 FPS (16 ms per frame)
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (m_runningGames[gameId]) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTime = elapsed.count();

        // Update the game state with the accurate deltaTime
        {
            std::lock_guard<std::mutex> lock(m_gameMutex);
            if (m_games.find(gameId) != m_games.end()) {
                m_games[gameId]->UpdateGame(deltaTime);
            }
            else {
                break;
            }
        }

        // Sleep to maintain a consistent frame rate (~60 FPS)
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> frameTime = endTime - currentTime;

        if (frameTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - frameTime);
        }
    }
}
