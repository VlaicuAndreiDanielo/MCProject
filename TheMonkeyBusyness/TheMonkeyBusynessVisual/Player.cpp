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
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" }
    );

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse || !jsonResponse.has("playerId")) {
        std::cerr << "Error: Login failed. Response: " << response.text << std::endl;
        return -1;
    }

    return jsonResponse["playerId"].i();
}

// Lobby-related methods
int Player::createLobby() {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/create_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"hostId":)" + std::to_string(id) + R"(})" }
    );

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse || !jsonResponse.has("lobbyId")) {
        std::cerr << "Error: Failed to create lobby. Response: " << response.text << std::endl;
        return -1;
    }

    return jsonResponse["lobbyId"].i();
}

bool Player::joinLobby(int lobbyId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/join_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    return response.status_code == 200;
}

bool Player::leaveLobby(int lobbyId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/leave_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    return response.status_code == 200;
}

bool Player::setReady(int lobbyId, bool isReady) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/set_ready" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(id) +
                  R"(,"isReady":)" + (isReady ? "true" : "false") + R"(})" }
    );

    return response.status_code == 200;
}

crow::json::wvalue Player::getActiveLobbies() {
    cpr::Response response = cpr::Get(cpr::Url{ serverUrl + "/get_active_lobbies" });
    return crow::json::load(response.text);
}

crow::json::wvalue Player::getLobbyDetails(int lobbyId) {
    cpr::Response response = cpr::Get(
        cpr::Url{ serverUrl + "/get_lobby_details" },
        cpr::Parameters{ {"lobbyId", std::to_string(lobbyId)} }
    );

    return crow::json::load(response.text);
}

// Game-related methods
int Player::startGame(int lobbyId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/start_game" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) +
                   R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse.has("gameId")) {
            return jsonResponse["gameId"].i();  // Return valid gameId
        }
    }

    std::cerr << "Error: Failed to start game. Response: " << response.text << std::endl;
    return -1;
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
