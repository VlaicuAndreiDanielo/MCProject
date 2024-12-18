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
    int CreateGameFromLobby(int lobbyId);
    void DeleteGame(int gameId);

    // Update loop
    bool StartGameLoop(int gameId);
    void StopGameLoop(int gameId);

    // Access game state
    GameState* GetGameState(int gameId);

private:
    std::unordered_map<int, GameState*> m_games;
    std::unordered_map<int, std::thread> m_gameThreads;
    std::unordered_map<int, bool> m_runningGames;
    std::mutex m_gameMutex;
    int m_nextGameId;

private:
    void GameLoop(int gameId);
};
