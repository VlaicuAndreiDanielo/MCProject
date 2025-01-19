#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include <memory>
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
    float GetDeltaTime();
    std::unordered_map<int, std::shared_ptr<GameState>> GetAllGames();
    // Access game state
    std::shared_ptr<GameState> GetGameState(int gameId);

private:
    std::unordered_map<int, std::shared_ptr<GameState>> m_games;
    std::unordered_map<int, std::thread> m_gameThreads;
    std::unordered_map<int, bool> m_runningGames;
    std::mutex m_gameMutex;
    float m_deltaTime;
    int m_nextGameId;

private:
    void GameLoop(int gameId);
};
