#pragma once
#include <unordered_map>
#include "Player.h"
#include "Arena.h"

enum class GameStatus {
    NotStarted,
    InProgress,
    GameOver
};

class GameState
{
public:
    GameState();
    GameState(const GameState&) = default; // Disallow copying
    GameState& operator=(const GameState&) = delete; // Disallow copy assignment
    GameState(GameState&&) = default; // Allow move construction
    GameState& operator=(GameState&&) = default; // Allow move assignment


    // Game Lifecycle
    void StartGame();
    void EndGame();
    bool IsGameOver() const;

    // Player Management
    void AddPlayer(int playerId);
    void RemovePlayer(int playerId);
    Player* GetPlayer(int playerId);
    Player InitializePlayer(int playerId);

    // Updates
    void ProcessMove(int playerId, const Vector2& movement, const Vector2& lookDirection);
    void ProcessShoot(int playerId, const Vector2& mousePosition);
    void UpdateGame(float deltaTime);

    // Serialization
    crow::json::wvalue toJson() const;
    crow::json::wvalue arenaToJson() const;

private:
    std::unordered_map<int, Player> players;
    Arena arena;
    GameStatus gameStatus;

    void UpdateBullets(float deltaTime);
    void CheckGameOver();
    crow::json::wvalue GameStatusToJson() const;
};
