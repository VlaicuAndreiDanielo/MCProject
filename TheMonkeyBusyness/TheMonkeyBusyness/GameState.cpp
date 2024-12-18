#include "GameState.h"
#include <stdexcept>
#include <crow.h>

GameState::GameState() : m_gameStatus(GameStatus::NotStarted) {}

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

Player GameState::InitializePlayer(int playerId)
{
    return Player(m_arena.GetSpawn().first * SQUARE_SIZE + SQUARE_SIZE / 2, m_arena.GetSpawn().second * SQUARE_SIZE + SQUARE_SIZE / 2, playerId);

}

void GameState::ProcessMove(int playerId, const Vector2& movement, const Vector2& lookDirection) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }
    if (GameObject* hit = m_raycast.Raycast(player->GetPosition(), movement, 15); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
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

    CheckGameOver();
}

void GameState::UpdateBullets(float deltaTime) {
    for (auto& [playerId, player] : m_players) {
        auto& bullets = player.m_weapon.GetActiveBullets();
        for (size_t i = 0; i < bullets.size();) {
            auto& bullet = bullets[i];
            bullet.Update(deltaTime);
            
            if (GameObject* hit = m_raycast.Raycast(bullet.GetPosition(), bullet.GetDirection(), 5); Tile * tempTile = dynamic_cast<Tile*>(hit)) {
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

void GameState::CheckGameOver() {
    int alivePlayers = 0;
    for (const auto& [playerId, player] : m_players) {
        if (player.IsAlive()) {
            ++alivePlayers;
        }
    }

    if (alivePlayers <= 1) {
        EndGame();
    }
}

void GameState::StartGame() {
    m_gameStatus = GameStatus::InProgress;
}

void GameState::EndGame() {
    m_gameStatus = GameStatus::GameOver;
}

bool GameState::IsGameOver() const {
    return m_gameStatus == GameStatus::GameOver;
}

crow::json::wvalue GameState::ToJson() const {
    crow::json::wvalue gameStateJson;

    // Serialize players
    crow::json::wvalue playersJson = crow::json::wvalue::list();
    size_t playerIndex = 0;
    for (const auto& [playerId, player] : m_players) {
        playersJson[playerIndex++] = player.ToJson();
    }
    gameStateJson["players"] = std::move(playersJson);

    // Serialize game status
    gameStateJson["gameStatus"] = static_cast<int>(m_gameStatus);

    return gameStateJson;
}

crow::json::wvalue GameState::ArenaToJson() const {
    crow::json::wvalue arenaData;
    arenaData["arena"] = m_arena.ToJson();
    return arenaData;
}

crow::json::wvalue GameState::GameStatusToJson() const {
    crow::json::wvalue statusJson;
    switch (m_gameStatus) {
    case GameStatus::NotStarted:
        statusJson["status"] = "NotStarted";
        break;
    case GameStatus::InProgress:
        statusJson["status"] = "InProgress";
        break;
    case GameStatus::GameOver:
        statusJson["status"] = "GameOver";
        break;
    }
    return statusJson;
}