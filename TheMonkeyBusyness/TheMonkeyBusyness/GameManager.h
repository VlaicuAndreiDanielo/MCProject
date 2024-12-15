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
    int createGameFromLobby(int lobbyId); // Creates a game from a lobby
    void deleteGame(int gameId);

    // Update loop
    bool startGameLoop(int gameId);
    void stopGameLoop(int gameId);

    // Access game state
    GameState* getGameState(int gameId);

private:
    std::unordered_map<int, GameState*> games; // Map of gameId to GameState pointers
    std::unordered_map<int, std::thread> gameThreads; // Map of gameId to threads
    std::unordered_map<int, bool> runningGames; // Map of gameId to running state
    std::mutex gameMutex; // Protect access to game maps

    void gameLoop(int gameId);
};
