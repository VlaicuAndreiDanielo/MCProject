#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Arena.h"
#include "Raycast.h"
#include <chrono>
#include "crow/json.h"

class GameState
{
public:
    GameState() : m_arena(std::make_shared<Arena>()), m_players(std::make_shared<std::unordered_map<int, Player>>()) {}
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
    void ProcessMove(int playerId, const Vector2<float>& movement, const Vector2<float>& lookDirection, float deltaTime);
    void ProcessShoot(int playerId, const Vector2<float>& mousePosition);
    void SpecialAbility(int playerId);
    void UpdateGame(float deltaTime);
    void SetResolution(int width, int height, int playerId);
    // Serialization
    crow::json::wvalue ToJson() const;
    crow::json::wvalue MapChangesToJson() const;
    crow::json::wvalue ArenaToJson() const;

private:
    std::shared_ptr<std::unordered_map<int, Player>> m_players;
    mutable std::vector<MapPosition> m_mapChanges;
    std::shared_ptr<Arena> m_arena;
    Cast m_raycast;

private:
    void UpdateBullets(float deltaTime);
};
