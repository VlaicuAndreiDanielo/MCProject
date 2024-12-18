#include "GameState.h"
#include <stdexcept>
#include <crow.h>

void GameState::AddPlayer(int playerId) {
    if (m_players.find(playerId) != m_players.end()) {
        throw std::runtime_error("Player ID already exists");
    }
    m_raycast.m_arena = &m_arena;
    m_players[playerId] = InitializePlayer(playerId);
}

void GameState::RemovePlayer(int playerId) {
    m_players.erase(playerId);
}

Player* GameState::GetPlayer(int playerId) {
    auto it = m_players.find(playerId);
    return (it != m_players.end()) ? &it->second : nullptr;
}

Player GameState::InitializePlayer(int playerId) {
    std::string playerName = GetPlayerNameFromDatabase(playerId);

    return Player(m_arena.GetSpawn().first * GameConfig::kTileSize + GameConfig::kTileSize / 2,
        m_arena.GetSpawn().second * GameConfig::kTileSize + GameConfig::kTileSize / 2,
        playerId, playerName);
}

std::string GameState::GetPlayerNameFromDatabase(int playerId)
{
    //TODO Andrei metoda asta primeste ca parametru ID ul playerului si ia din baza de date numele asociat si il returneaza
    return "Mario";
}

void GameState::ProcessMove(int playerId, const Vector2& movement, const Vector2& lookDirection) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }
    if (GameObject* hit = m_raycast.Raycast(player->GetPosition(), movement, GameConfig::kRaycastRange); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
        if (tempTile->getType() != TileType::DestructibleWall && tempTile->getType() != TileType::IndestructibleWall && tempTile->getType() != TileType::FakeDestructibleWall) {
            player->UpdatePosition(movement);
        }
    }
    player->UpdateRotation(lookDirection);
}

void GameState::ProcessShoot(int playerId, const Vector2& mousePosition) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }

    player->Shoot(mousePosition);
}


void GameState::UpdateGame(float deltaTime)
{
    for (auto& [playerId, player] : m_players) {
        player.Update(deltaTime);
    }

    UpdateBullets(deltaTime);
}

void GameState::UpdateBullets(float deltaTime) {
    for (auto& [playerId, player] : m_players) {
        auto& bullets = player.m_weapon.GetActiveBullets();
        for (size_t i = 0; i < bullets.size();) {
            auto& bullet = bullets[i];
            bullet.Update(deltaTime);
            
            if (GameObject* hit = m_raycast.Raycast(bullet.GetPosition(), bullet.GetDirection(), GameConfig::kBulletRaycastRange); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
                //TODO Rob pentru tipurile de ziduri care se sparg, in momentul in care s-au spart adauga coordonatele in containerul de mai jos
                //m_mapChanges.emplace_back(x, y)
                //Poti sa stergi urmatorul if daca nu l-am facut bine,l-am facut repede ca sa testez
                if (tempTile->getType() == TileType::DestructibleWall)
                {
                    int x = static_cast<int>((bullet.GetPosition().x + GameConfig::kTileSize / 2) / GameConfig::kTileSize);
                    int y = static_cast<int>((bullet.GetPosition().y + GameConfig::kTileSize / 2) / GameConfig::kTileSize);

                    m_mapChanges.push_back({x, y});

                    //TODO Rob transforma in arena de pe server tile de la perete care se poate distruge la tile in care se transforma un perete spart
                }
                
                if (tempTile->getType() != TileType::DestructibleWall && tempTile->getType() != TileType::IndestructibleWall && tempTile->getType() != TileType::FakeDestructibleWall) {
                    ++i;
                }
                else {
                    player.m_weapon.deactivateBullet(i);
                }

            }
        }
    }
}

bool GameState::IsGameOver() const {
    return m_players.size() <= 1; // Game is over when one or no players are left
}

crow::json::wvalue GameState::ToJson() const {
    crow::json::wvalue gameStateJson;

    // Serialize players (this includes their bullets)
    crow::json::wvalue playersJson = crow::json::wvalue::list();
    size_t playerIndex = 0;
    for (const auto& [playerId, player] : m_players) {
        playersJson[playerIndex++] = player.ToJson();
    }
    gameStateJson["players"] = std::move(playersJson);

    // Serialize map changes
    gameStateJson["mapChanges"] = MapChangesToJson();
    m_mapChanges.clear();

    // Serialize game status
    gameStateJson["isGameOver"] = IsGameOver();

    return gameStateJson;
}

crow::json::wvalue GameState::MapChangesToJson() const
{
    crow::json::wvalue changes = crow::json::wvalue::list();
    size_t index = 0;
    for (const auto& [x, y] : m_mapChanges) {
        changes[index]["x"] = x;
        changes[index]["y"] = y;
        ++index;
    }
    return changes;
}

crow::json::wvalue GameState::ArenaToJson() const {
    crow::json::wvalue arenaData;
    arenaData["arena"] = m_arena.ToJson();
    return arenaData;
}