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
    int CreateGameFromLobby(int lobbyId); // Creates a game from a lobby
    void deleteGame(int gameId);

    // Update loop
    bool StartGameLoop(int gameId);
    void StopGameLoop(int gameId);

    // Access game state
    GameState* GetGameState(int gameId);

private:
    std::unordered_map<int, GameState*> m_games; // Map of gameId to GameState pointers
    std::unordered_map<int, std::thread> m_gameThreads; // Map of gameId to threads
    std::unordered_map<int, bool> m_runningGames; // Map of gameId to running state
    std::mutex m_gameMutex; // Protect access to game maps

    void gameLoop(int gameId);
};
