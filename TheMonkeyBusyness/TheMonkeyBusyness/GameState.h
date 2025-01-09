#pragma once
#include <unordered_map>
#include "Player.h"
#include "Arena.h"
#include "Raycast.h"
#include <chrono>
class GameState
{
public:
    GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(GameState&&) = default;
    GameState& operator=(GameState&&) = default;
    
    // Game Lifecycle
    bool IsGameOver() const;
    int CalculateDeltaTime(int playerId);
    // Player Management
    void AddPlayer(int playerId);
    void RemovePlayer(int playerId);
    Player* GetPlayer(int playerId);
    Player InitializePlayer(int playerId);
    std::string GetPlayerNameFromDatabase(int playerId);

    // Updates
    void ProcessMove(int playerId, const Vector2& movement, const Vector2& lookDirection);
    void ProcessShoot(int playerId, const Vector2& mousePosition);
    void UpdateGame(float deltaTime);

    // Serialization
    crow::json::wvalue ToJson() const;
    crow::json::wvalue MapChangesToJson() const;
    crow::json::wvalue ArenaToJson() const;

private:
    std::unordered_map<int, Player> m_players;
    std::unordered_map<int, std::chrono::steady_clock::time_point > m_playerDeltaTime;
    mutable std::vector<MapPosition> m_mapChanges;
    Arena m_arena;
    Cast m_raycast;

private:
    void UpdateBullets(float deltaTime);
};
