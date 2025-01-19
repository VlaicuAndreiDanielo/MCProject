#include "GameState.h"
#include "UserDatabase.h"
#include <stdexcept>
#include <crow.h>
#include <chrono>
#include <iostream>

void GameState::AddPlayer(int playerId) {
    if (m_players->find(playerId) != m_players->end()) {
        throw std::runtime_error("Player ID already exists");
    }
    m_raycast.m_arena = m_arena;
    m_raycast.m_players = m_players;
    (*m_players)[playerId] = InitializePlayer(playerId);
}

void GameState::RemovePlayer(int playerId) {
    m_players->erase(playerId);
}

Player* GameState::GetPlayer(int playerId) {
    auto it = m_players->find(playerId);
    return (it != m_players->end()) ? &(it->second) : nullptr;
}

Player GameState::InitializePlayer(int playerId) {
    std::string playerName = GetPlayerNameFromDatabase(playerId);
    bool isOverlapping = false;
    std::pair<int, int> spawn;
    do {
        spawn = m_arena->GetSpawn();
        isOverlapping = false; // Reset isOverlapping before checking
        for (const auto& [pid, player] : *m_players)
        {
            if (player.GetPosition().x == spawn.first * GameConfig::kTileSize + GameConfig::kTileSize / 2
                && player.GetPosition().y == spawn.second * GameConfig::kTileSize + GameConfig::kTileSize / 2) {
                isOverlapping = true;
                break;
            }
        }
    } while (isOverlapping);

    return Player(spawn.first * GameConfig::kTileSize + GameConfig::kTileSize / 2,
        spawn.second * GameConfig::kTileSize + GameConfig::kTileSize / 2,
        playerId, playerName);
}

std::string GameState::GetPlayerNameFromDatabase(int playerId) {
    UserDatabase database("userdatabase.db");

    // Obține username-ul din baza de date pe baza playerId-ului
    std::string username = database.GetUsernameById(playerId);

    // Verifică dacă username-ul a fost găsit
    if (username.empty()) {
        std::cerr << "Player ID " << playerId << " not found in the database." << std::endl;
        return "UnknownPlayer"; // Returnează un nume generic dacă nu există în baza de date
    }

    return username; // Returnează numele utilizatorului
}

bool GameState::IsGameOver() const {
    return m_players->size() <= 1; // Game is over when one or no players are left
}

void GameState::ProcessMove(int playerId, const Vector2<float>& movement, const Vector2<float>& lookDirection, float deltaTime) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }

    if (GameObject* hit = m_raycast.Raycast(player->GetPosition(), movement, GameConfig::kRaycastRange, *player); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
        if (tempTile->getType() != TileType::DestructibleWall && tempTile->getType() != TileType::IndestructibleWall && tempTile->getType() != TileType::FakeDestructibleWall) {
            player->UpdatePosition(movement, deltaTime);
        }
        Character* character = player->GetCharacter();
        if (!player->IsSlowed())
        {
            player->SetOldSpeed(character->GetSpeed());
        }
        bool isSubmerged = false;
        if (tempTile->getType() == TileType::Water || tempTile->getType() == TileType::Lava)
        {
            isSubmerged = true;
            player->SetisSlowed(true);
            character->SetSpeed(player->GetOldSpeed() / 2);
            if (tempTile->getType() == TileType::Lava) {
                if (!player->IsUnderDot()) {
                        player->StartDoT(3.0f); // DoT active for 3s
                }
            }
        }
        else if (!isSubmerged && player->IsSlowed())
        {
            isSubmerged = false;
            character->SetSpeed(player->GetOldSpeed());
            player->SetisSlowed(false);
            if (player->IsUnderDot()) {
                player->StopDoT(); // Stop the DoT if the player leaves the lava
            }
        }
    }
    player->UpdateRotation(lookDirection);
}

void GameState::ProcessShoot(int playerId, const Vector2<float>& mousePosition) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }

    player->Shoot(mousePosition);
}

void GameState::SpecialAbility(int playerId)
{
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }
    player->ActivateAbility();
}

void GameState::UpdateGame(float deltaTime)
{
    for (auto& [playerId, player] : *m_players) {
        player.Update(deltaTime);
        player.UpdateDot();
    }

    UpdateBullets(deltaTime);
}

void GameState::SetResolution(int width, int height, int playerId)
{
    (*m_players)[playerId].SetScreenSize(width, height);
}

void GameState::UpdateBullets(float deltaTime) {
    for (auto& [playerId, player] : *m_players) {
        auto& bullets = player.m_weapon.GetActiveBullets();
        for (size_t i = 0; i < bullets.size();) {
            auto& bullet = bullets[i];
            bullet.Update(deltaTime);
            Vector2<float> RayCastLocation;
            if (GameObject* hit = m_raycast.Raycast(bullet.GetPosition(), bullet.GetDirection(), GameConfig::kBulletRaycastRange, player); Player * tempPlayer = dynamic_cast<Player*>(hit)) {
                tempPlayer->Damage(bullet.GetDamage());
                player.m_weapon.deactivateBullet(i);
            }
            if (GameObject* hit = m_raycast.Raycast(bullet.GetPosition(), bullet.GetDirection(), GameConfig::kBulletRaycastRange, player, RayCastLocation); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
                TileType tempType = tempTile->getType();
                if (tempType == TileType::DestructibleWall || tempType == TileType::FakeDestructibleWall)
                {
                    tempTile->takeDamage(bullet.GetDamage());
                    if (tempTile->getHP() <= 0) {
                        int x = (std::floor(RayCastLocation.x / GameConfig::kTileSize));
                        int y = (std::floor(RayCastLocation.y / GameConfig::kTileSize));
                        if (tempType == TileType::FakeDestructibleWall) {
                            // Apply damage to players and destructible tiles in the area around the tile
                            for (int dx = -1; dx <= 1; ++dx) {
                                for (int dy = -1; dy <= 1; ++dy) {
                                    int checkX = y + dy;
                                    int checkY = x + dx;

                                    for (auto& [playerId, player] : *m_players) {
                                        // Check if the player's position overlaps with the tile's surrounding area
                                        if ((int)player.GetPosition().y / GameConfig::kTileSize == checkX &&
                                            (int)player.GetPosition().x / GameConfig::kTileSize == checkY) {
                                            // Apply damage to the player
                                            player.Damage(40);
                                        }
                                    }

                                    Tile* surroundingTile = &m_arena->GetTile(checkX, checkY);
                                    if (surroundingTile->getType() == TileType::FakeDestructibleWall || surroundingTile->getType() == TileType::DestructibleWall)
                                    {
                                        surroundingTile->takeDamage(30);
                                        m_mapChanges.push_back({ checkY, checkX });
                                    }
                                }
                            }
                        }
                        m_mapChanges.push_back({ x, y });
                        player.m_weapon.deactivateBullet(i);
                    }
                    else {
                        player.m_weapon.deactivateBullet(i);
                    }
                    //TODO Rob transforma in arena de pe server tile de la perete care se poate distruge la tile in care se transforma un perete spart
                }
                else {
                    if (tempTile->getType() == TileType::IndestructibleWall) {
                        player.m_weapon.deactivateBullet(i);
                    }
                    else {
                        ++i;
                    }
                }
            }
        }
    }
}

crow::json::wvalue GameState::ToJson() const {
    crow::json::wvalue gameStateJson;

    // Serialize players (this includes their bullets)
    crow::json::wvalue playersJson = crow::json::wvalue::list();
    size_t playerIndex = 0;
    for (const auto& [playerId, player] : *m_players) {
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
    arenaData["arena"] = m_arena->ToJson();
    return arenaData;
}
