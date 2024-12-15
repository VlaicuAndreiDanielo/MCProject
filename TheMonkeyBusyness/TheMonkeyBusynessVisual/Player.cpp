#include "Player.h"
#include <iostream>
#include <fstream>

Player::Player(int id, const std::string& serverUrl)
    : id(id), serverUrl(serverUrl), position({ 400, 300 }), direction({ 0.0f, 0.0f }), health(100) {}

// Static method to log in
int Player::logIn(const std::string& serverUrl, const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/login" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" });

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse || !jsonResponse.has("playerId")) {
        std::cerr << "Error: Login failed. Response: " << response.text << std::endl;
        return -1;
    }

    return jsonResponse["playerId"].i();
}

// Static method to create a game
int Player::createGame(const std::string& serverUrl, int playerId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/create_game" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"playerId":)" + std::to_string(playerId) + R"(})" });

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse || !jsonResponse.has("gameId")) {
        std::cerr << "Error: Game creation failed. Response: " << response.text << std::endl;
        return -1;
    }

    return jsonResponse["gameId"].i();
}

// Start the game loop
void Player::startGame(int gameId) {
    auto response = cpr::Post(
        cpr::Url{ serverUrl + "/start_game_loop" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"gameId":)" + std::to_string(gameId) + R"(})" });

    if (response.status_code != 200) {
        std::cerr << "Error: Failed to start the game loop. Response: " << response.text << std::endl;
    }
}

// Accessors and mutators
int Player::getId() const { return id; }
int Player::getGameId() const { return gameId; }
std::string Player::getName() const { return name; }
Position Player::getPosition() const { return position; }
Direction Player::getDirection() const { return direction; }
const std::string& Player::getServerUrl() const { return serverUrl; }

void Player::setGameId(int id) { gameId = id; }
void Player::setName(const std::string& playerName) { name = playerName; }
void Player::setPosition(const Position& pos) { position = pos; }
void Player::setDirection(const Direction& dir) { direction = dir; }
void Player::setHealth(int hp) { health = hp; }


