#include "Player.h"
#include <iostream>
#include <fstream>

Player::Player(int id, const std::string& serverUrl)
    : id{ id },
    gameId{ -1 },            // Default to -1 to indicate no game
    lobbyId{ -1 },           // Default to -1 to indicate no lobby
    isReady{ false },        // Default to not ready
    isHost{ false },          // Default to not host
    name{ "" },              // Empty name by default
    serverUrl{ serverUrl },  // Initialize with provided server URL
    position{ 0.0f, 0.0f },  // Default position at (0, 0)
    direction{ 0.0f, 0.0f }, // Default direction at (0, 0)
    health{ 100 }            // Default health value
{}

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

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse && jsonResponse.has("lobbyId")) {
            lobbyId = jsonResponse["lobbyId"].i();
            setHost(true);
            return lobbyId;
        }
    }
    return -1;
}



bool Player::joinLobby(int lobbyId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/join_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    return response.status_code == 200;
}

bool Player::leaveLobby() {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/leave_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    if (response.status_code == 200) {
        lobbyId = -1;       // Reset lobbyId
        setHost(false);     
        return true;
    }

    std::cerr << "Error: Failed to leave lobby. Response: " << response.text << std::endl;
    return false;
}



bool Player::setReady() {
    isReady = !isReady; // Toggle ready state

    // Send the ready state to the server
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/set_ready" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) +
                    R"(,"playerId":)" + std::to_string(id) +
                    R"(,"isReady":)" + (isReady ? "true" : "false") + R"(})" }
    );

    if (response.status_code == 200) {
        return true; // Successfully updated ready state
    }
    else {
        std::cerr << "Failed to set ready. Server response: " << response.text << std::endl;
        // Roll back the toggle on failure
        isReady = !isReady;
        return false;
    }
}


crow::json::wvalue Player::getActiveLobbies() {
    cpr::Response response = cpr::Get(cpr::Url{ serverUrl + "/get_active_lobbies" });
    if (response.status_code != 200) {
        std::cerr << "Error: Failed to fetch active lobbies. Response: " << response.text << std::endl;
        return {};
    }
    return crow::json::load(response.text);
}


crow::json::wvalue Player::getLobbyDetails() {
    cpr::Response response = cpr::Get(
        cpr::Url{ serverUrl + "/get_lobby_details" },
        cpr::Parameters{ {"lobbyId", std::to_string(lobbyId)} }
    );

    if (response.status_code == 200) {
        return crow::json::load(response.text);
    }

    std::cerr << "Error: Failed to fetch lobby details. Response: " << response.text << std::endl;
    return {};
}


// Game-related methods
int Player::startGame() {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/start_game" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) +
                    R"(,"playerId":)" + std::to_string(id) + R"(})" }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse.has("gameId")) {
            gameId = jsonResponse["gameId"].i(); // Assign the gameId to this player
            return gameId;
        }
        else {
            std::cerr << "Error: No gameId received in response: " << response.text << std::endl;
            return -1; // Indicate failure
        }
    }
    else {
        std::cerr << "Error: Failed to start game. Server response: " << response.text << std::endl;
        return -1; // Indicate failure
    }
}



// Accessors and mutators
int Player::getId() const { return id; }
int Player::getGameId() const { return gameId; }
int Player::getLobbyId() const { return lobbyId; }
bool Player::getIsReady() const { return isReady; }
bool Player::getIsHost() const { return isHost; }
std::string Player::getName() const { return name; }
Position Player::getPosition() const { return position; }
Direction Player::getDirection() const { return direction; }
const std::string& Player::getServerUrl() const { return serverUrl; }

void Player::setGameId(int id) { gameId = id; }
void Player::setLobbyId(int newLobbyId) { lobbyId = newLobbyId; }
void Player::setHost(bool hostStatus) { isHost = hostStatus; }
void Player::setName(const std::string& playerName) { name = playerName; }
void Player::setPosition(const Position& pos) { position = pos; }
void Player::setDirection(const Direction& dir) { direction = dir; }
void Player::setHealth(int hp) { health = hp; }
