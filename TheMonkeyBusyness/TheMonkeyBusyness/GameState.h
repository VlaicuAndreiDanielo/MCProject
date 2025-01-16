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
    // Player Management
    void AddPlayer(int playerId);
    void RemovePlayer(int playerId);
    Player* GetPlayer(int playerId);
    Player InitializePlayer(int playerId);
    std::string GetPlayerNameFromDatabase(int playerId);

    // Updates
    void ProcessMove(int playerId, const Vector2<float>& movement, const Vector2<float>& lookDirection,float deltaTime);
    void ProcessShoot(int playerId, const Vector2<float>& mousePosition);
    void SpecialAbility(int playerId);
    void UpdateGame(float deltaTime);
    void SetResolution(int width, int height,int playerId);
    // Serialization
    crow::json::wvalue ToJson() const;
    crow::json::wvalue MapChangesToJson() const;
    crow::json::wvalue ArenaToJson() const;

private:
    std::unordered_map<int, Player> m_players;
    mutable std::vector<MapPosition> m_mapChanges;
    Arena m_arena;
    Cast m_raycast;

private:
    void UpdateBullets(float deltaTime);
};
