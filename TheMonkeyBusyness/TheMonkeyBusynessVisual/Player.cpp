#include "Player.h"
#include <iostream>
#include <fstream>

Player::Player(int id, const std::string& serverUrl)
    : m_id{ id },
    m_gameId{ PlayerConfig::kNoGameId },
    m_lobbyId{ PlayerConfig::kNoLobbyId },
    m_isReady{ false },
    m_isHost{ false },
    m_name{ "" }, 
    m_serverUrl{ serverUrl }, 
    m_position{ PlayerConfig::kDefaultPositionX, PlayerConfig::kDefaultPositionY },
    m_direction{ PlayerConfig::kDefaultDirectionX, PlayerConfig::kDefaultDirectionY },
    m_health{ PlayerConfig::kDefaultHealth }
{}

// Static method to log in
int Player::LogIn(const std::string& serverUrl, const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/login" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" }
    );

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse || !jsonResponse.has("playerId")) {
        return -1;
    }

    return jsonResponse["playerId"].i();
}

int Player::SignIn(const std::string& serverUrl, const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/signin" }, // Assuming the Crow route for sign-in is `/signin`
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" }
    );

    // Check if the response is successful
    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse && jsonResponse.has("playerId")) {
            return jsonResponse["playerId"].i(); // Return the player's ID on successful sign-in
        }
    }

    // Return -1 if the sign-in failed
    return -1;
}


// Lobby-related methods
int Player::CreateLobby() {
    cpr::Response response = cpr::Post(
        cpr::Url{ m_serverUrl + "/create_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"hostId":)" + std::to_string(m_id) + R"(})" }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse && jsonResponse.has("lobbyId")) {
            m_lobbyId = jsonResponse["lobbyId"].i();
            SetHost(true);
            return m_lobbyId;
        }
    }
    return -1;
}



bool Player::JoinLobby(int lobbyId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ m_serverUrl + "/join_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(lobbyId) + R"(,"playerId":)" + std::to_string(m_id) + R"(})" }
    );

    return response.status_code == 200;
}

bool Player::LeaveLobby() {
    cpr::Response response = cpr::Post(
        cpr::Url{ m_serverUrl + "/leave_lobby" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(m_lobbyId) + R"(,"playerId":)" + std::to_string(m_id) + R"(})" }
    );

    if (response.status_code == 200) {
        m_lobbyId = -1;       // Reset lobbyId
        SetHost(false);     
        return true;
    }

    return false;
}



bool Player::SetReady() {
    m_isReady = !m_isReady; // Toggle ready state

    // Send the ready state to the server
    cpr::Response response = cpr::Post(
        cpr::Url{ m_serverUrl + "/set_ready" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(m_lobbyId) +
                    R"(,"playerId":)" + std::to_string(m_id) +
                    R"(,"isReady":)" + (m_isReady ? "true" : "false") + R"(})" }
    );

    if (response.status_code == 200) {
        return true; // Successfully updated ready state
    }
    else {
        // Roll back the toggle on failure
        m_isReady = !m_isReady;
        return false;
    }
}


crow::json::wvalue Player::GetActiveLobbies() {
    cpr::Response response = cpr::Get(cpr::Url{ m_serverUrl + "/get_active_lobbies" });
    if (response.status_code != 200) {
        return {};
    }
    return crow::json::load(response.text);
}


crow::json::wvalue Player::GetLobbyDetails(int lobbyId) {
    cpr::Response response = cpr::Get(
        cpr::Url{ m_serverUrl + "/get_lobby_details" },
        cpr::Parameters{ {"lobbyId", std::to_string(lobbyId)} }
    );

    if (response.status_code == 200) {
        return crow::json::load(response.text);
    }

    return {};
}


// Game-related methods
int Player::StartGame() {
    cpr::Response response = cpr::Post(
        cpr::Url{ m_serverUrl + "/start_game" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"lobbyId":)" + std::to_string(m_lobbyId) +
                    R"(,"playerId":)" + std::to_string(m_id) + R"(})" }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse.has("gameId")) {
            m_gameId = jsonResponse["gameId"].i(); // Assign the gameId to this player
            return m_gameId;
        }
        else {
            return -1; // Indicate failure
        }
    }
    else {
        return -1; // Indicate failure
    }
}



// Accessors and mutators
int Player::GetId() const { return m_id; }
int Player::GetGameId() const { return m_gameId; }
int Player::GetLobbyId() const { return m_lobbyId; }
bool Player::GetIsReady() const { return m_isReady; }
bool Player::GetIsHost() const { return m_isHost; }
std::string Player::GetName() const { return m_name; }
Position Player::GetPosition() const { return m_position; }
Direction Player::GetDirection() const { return m_direction; }
const std::string& Player::GetServerUrl() const { return m_serverUrl; }
int Player::GetHealth() const { return m_health; }

void Player::SetGameId(int id) { m_gameId = id; }
void Player::SetLobbyId(int newLobbyId) { m_lobbyId = newLobbyId; }
void Player::SetHost(bool hostStatus) { m_isHost = hostStatus; }
void Player::SetName(const std::string& playerName) { m_name = playerName; }
void Player::SetPosition(const Position& pos) { m_position = pos; }
void Player::SetDirection(const Direction& dir) { m_direction = dir; }
void Player::SetHealth(int hp) { m_health = hp; }
