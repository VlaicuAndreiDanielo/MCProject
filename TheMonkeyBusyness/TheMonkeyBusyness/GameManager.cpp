#include "GameManager.h"
#include <chrono>
#include <iostream>

GameManager::GameManager() {}

GameManager::~GameManager() {
    for (auto& [gameId, _] : games) {
        StopGameLoop(gameId);
    }
    for (auto& [gameId, gameState] : games) {
        delete gameState;
    }
}

int GameManager::CreateGame() {
    std::lock_guard<std::mutex> lock(gameMutex);

    static int nextGameId = 1;
    int gameId = nextGameId++;
    games[gameId] = new GameState();
    runningGames[gameId] = false;

    return gameId;
}

void GameManager::DeleteGame(int gameId) {
    StopGameLoop(gameId);

    std::lock_guard<std::mutex> lock(gameMutex);
    if (games.find(gameId) != games.end()) {
        delete games[gameId];
        games.erase(gameId);
        gameThreads.erase(gameId);
        runningGames.erase(gameId);

    }
}

bool GameManager::AddPlayerToGame(int gameId, int playerId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (games.find(gameId) == games.end()) {
        return false;
    }

    games[gameId]->AddPlayer(playerId);
    return true;
}

void GameManager::RemovePlayerFromGame(int gameId, int playerId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (games.find(gameId) != games.end()) {
        games[gameId]->RemovePlayer(playerId);

    }
}

GameState* GameManager::GetGameState(int gameId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (games.find(gameId) != games.end()) {
        return games[gameId];
    }
    return nullptr;
}

bool GameManager::StartGameLoop(int gameId) {
    std::lock_guard<std::mutex> lock(gameMutex);

    if (runningGames[gameId]) {
        return true;  // Game loop is already running
    }

    runningGames[gameId] = true;

    try {
        gameThreads[gameId] = std::thread(&GameManager::GameLoop, this, gameId);
        return true;  // Successfully started the game loop
    }
    catch (const std::exception& e) {
        runningGames[gameId] = false;  // Reset the state if starting the thread fails
        return false;  // Indicate failure
    }
}


void GameManager::StopGameLoop(int gameId) {
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


void GameManager::GameLoop(int gameId) {
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

        // Log the GameLoop iteration with deltaTime
        {
            std::ofstream logFile("server_log.txt", std::ios::app);
            logFile << "GameLoop iteration completed for gameId=" << gameId
                << ", deltaTime=" << deltaTime << std::endl;
        }

        // Calculate the time taken to process the frame
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> frameTime = endTime - currentTime;

        // Sleep to maintain a consistent frame rate (~60 FPS)
        if (frameTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - frameTime);
        }
    }

}
