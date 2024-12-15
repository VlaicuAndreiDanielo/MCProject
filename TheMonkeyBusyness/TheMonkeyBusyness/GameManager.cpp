#include "GameManager.h"
#include "LobbyManager.h"
#include <chrono>
#include <iostream>

extern LobbyManager lobbyManager;

GameManager::GameManager() {}

GameManager::~GameManager() {
    for (auto& [gameId, _] : games) {
        stopGameLoop(gameId);
    }
    for (auto& [gameId, gameState] : games) {
        delete gameState;
    }
}

int GameManager::createGameFromLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    static int nextGameId = 1;
    int gameId = nextGameId++;

    Lobby* lobby = lobbyManager.getLobby(lobbyId);
    if (!lobby) {
        throw std::runtime_error("Lobby not found");
    }

    // Create game state and add all lobby players
    GameState* gameState = new GameState();
    const auto& playersMap = lobby->getPlayers(); // Access players and their ready statuses
    for (const auto& [playerId, isReady] : playersMap) {
        gameState->AddPlayer(playerId); // Add each player to the game
    }

    games[gameId] = gameState;
    runningGames[gameId] = false;

    return gameId;
}

void GameManager::deleteGame(int gameId) {
    stopGameLoop(gameId);

    std::lock_guard<std::mutex> lock(gameMutex);
    if (games.find(gameId) != games.end()) {
        delete games[gameId];
        games.erase(gameId);
        gameThreads.erase(gameId);
        runningGames.erase(gameId);
    }
}

bool GameManager::startGameLoop(int gameId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (runningGames[gameId]) {
        return true;  // Game loop is already running
    }

    runningGames[gameId] = true;

    try {
        gameThreads[gameId] = std::thread(&GameManager::gameLoop, this, gameId);
        return true;  // Successfully started the game loop
    }
    catch (const std::exception& e) {
        runningGames[gameId] = false;  // Reset the state if starting the thread fails
        return false;  // Indicate failure
    }
}

void GameManager::stopGameLoop(int gameId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (!runningGames[gameId]) {
        return;
    }

    runningGames[gameId] = false;
    if (gameThreads[gameId].joinable()) {
        gameThreads[gameId].join();
    }
    gameThreads.erase(gameId);
}

GameState* GameManager::getGameState(int gameId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (games.find(gameId) != games.end()) {
        return games[gameId];
    }
    return nullptr;
}

void GameManager::gameLoop(int gameId) {
    const std::chrono::milliseconds frameDuration(16); // ~60 FPS (16 ms per frame)
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (runningGames[gameId]) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTime = elapsed.count();

        // Update the game state with the accurate deltaTime
        {
            std::lock_guard<std::mutex> lock(gameMutex);
            if (games.find(gameId) != games.end()) {
                games[gameId]->UpdateGame(deltaTime);
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
