#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include "GameState.h"

class GameManager {
public:
    GameManager();
    ~GameManager();

    // Game lifecycle
    int CreateGame();
    void DeleteGame(int gameId);

    // Player management
    bool AddPlayerToGame(int gameId, int playerId);
    void RemovePlayerFromGame(int gameId, int playerId);

    // Update loop
    bool StartGameLoop(int gameId);
    void StopGameLoop(int gameId);

    // Access game state
    GameState* GetGameState(int gameId);

private:
    std::unordered_map<int, GameState*> games; // Map of gameId to GameState pointers
    std::unordered_map<int, std::thread> gameThreads; // Map of gameId to threads
    std::unordered_map<int, bool> runningGames; // Map of gameId to running state
    std::mutex gameMutex; // Protect access to game maps

    void GameLoop(int gameId);
};
