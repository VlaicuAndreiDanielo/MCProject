#include "GameState.h"
#include <stdexcept>
#include <crow.h>

GameState::GameState() : gameStatus(GameStatus::NotStarted) {}

void GameState::AddPlayer(int playerId) {
    if (players.find(playerId) != players.end()) {
        throw std::runtime_error("Player ID already exists");
    }
   
    players[playerId] = InitializePlayer(playerId);
}

void GameState::RemovePlayer(int playerId) {
    players.erase(playerId);
}

Player* GameState::GetPlayer(int playerId) {
    auto it = players.find(playerId);
    return (it != players.end()) ? &it->second : nullptr;
}

Player GameState::InitializePlayer(int playerId)
{
    return Player(Arena::Instance().GetSpawn().first * SQUARE_SIZE + SQUARE_SIZE / 2, Arena::Instance().GetSpawn().second * SQUARE_SIZE + SQUARE_SIZE / 2, playerId);

}

void GameState::ProcessMove(int playerId, const Vector2& movement, const Vector2& lookDirection) {
    Player* player = GetPlayer(playerId);
    if (!player) {
        return; // Player not found
    }

    player->UpdatePosition(movement);
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
    for (auto& [playerId, player] : players) {
        player.Update(deltaTime);
    }

    UpdateBullets(deltaTime);

    CheckGameOver();
}

void GameState::UpdateBullets(float deltaTime) {
    for (auto& [playerId, player] : players) {
        auto& bullets = player.m_weapon.GetActiveBullets();
        for (size_t i = 0; i < bullets.size();) {
            auto& bullet = bullets[i];
            bullet.Update(deltaTime);

            // TODO: Add collision detection logic and register map changes if bullets hit walls
            if (/* collision condition */ false) {
                player.m_weapon.deactivateBullet(i);
            }
            else {
                ++i;
            }
        }
    }
}

void GameState::CheckGameOver() {
    int alivePlayers = 0;
    for (const auto& [playerId, player] : players) {
        if (player.IsAlive()) {
            ++alivePlayers;
        }
    }

    if (alivePlayers <= 1) {
        EndGame();
    }
}

void GameState::StartGame() {
    gameStatus = GameStatus::InProgress;
}

void GameState::EndGame() {
    gameStatus = GameStatus::GameOver;
}

bool GameState::IsGameOver() const {
    return gameStatus == GameStatus::GameOver;
}

crow::json::wvalue GameState::toJson() const {


    auto start = std::chrono::high_resolution_clock::now(); //for logging

    crow::json::wvalue gameStateJson;

    // Serialize players
    crow::json::wvalue playersJson = crow::json::wvalue::list();
    size_t playerIndex = 0;
    for (const auto& [playerId, player] : players) {
        playersJson[playerIndex++] = player.toJson();
    }
    gameStateJson["players"] = std::move(playersJson);

    // Serialize game status
    gameStateJson["gameStatus"] = static_cast<int>(gameStatus);


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::ofstream logFile("server_log.txt", std::ios::app);
    logFile << "Processed GameState::toJson which is called in every game_state in " << duration.count() << " seconds." << std::endl;

    return gameStateJson;
}

crow::json::wvalue GameState::arenaToJson() const {
    crow::json::wvalue arenaData;
    arenaData["arena"] = arena.toJson();
    return arenaData;
}

crow::json::wvalue GameState::GameStatusToJson() const {
    crow::json::wvalue statusJson;
    switch (gameStatus) {
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