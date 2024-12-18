#include "Player.h"
#include <iostream>
#include <fstream>

Player::Player(int id, const std::string& serverUrl)
    : m_id{ id },
    m_gameId{ -1 },            // Default to -1 to indicate no game
    m_lobbyId{ -1 },           // Default to -1 to indicate no lobby
    m_isReady{ false },        // Default to not ready
    m_isHost{ false },          // Default to not host
    m_name{ "" },              // Empty name by default
    m_serverUrl{ serverUrl },  // Initialize with provided server URL
    m_position{ 0.0f, 0.0f },  // Default position at (0, 0)
    m_direction{ 0.0f, 0.0f }, // Default direction at (0, 0)
    m_health{ 100 }            // Default health value
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
        std::cerr << "Error: Login failed. Response: " << response.text << std::endl;
        return -1;
    }

    return jsonResponse["playerId"].i();
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

    std::cerr << "Error: Failed to leave lobby. Response: " << response.text << std::endl;
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
        std::cerr << "Failed to set ready. Server response: " << response.text << std::endl;
        // Roll back the toggle on failure
        m_isReady = !m_isReady;
        return false;
    }
}


crow::json::wvalue Player::GetActiveLobbies() {
    cpr::Response response = cpr::Get(cpr::Url{ m_serverUrl + "/get_active_lobbies" });
    if (response.status_code != 200) {
        std::cerr << "Error: Failed to fetch active lobbies. Response: " << response.text << std::endl;
        return {};
    }
    return crow::json::load(response.text);
}


crow::json::wvalue Player::GetLobbyDetails() {
    cpr::Response response = cpr::Get(
        cpr::Url{ m_serverUrl + "/get_lobby_details" },
        cpr::Parameters{ {"lobbyId", std::to_string(m_lobbyId)} }
    );

    if (response.status_code == 200) {
        return crow::json::load(response.text);
    }

    std::cerr << "Error: Failed to fetch lobby details. Response: " << response.text << std::endl;
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
int Player::GetId() const { return m_id; }
int Player::GetGameId() const { return m_gameId; }
int Player::GetLobbyId() const { return m_lobbyId; }
bool Player::GetIsReady() const { return m_isReady; }
bool Player::GetIsHost() const { return m_isHost; }
std::string Player::GetName() const { return m_name; }
Position Player::GetPosition() const { return m_position; }
Direction Player::GetDirection() const { return m_direction; }
const std::string& Player::GetServerUrl() const { return m_serverUrl; }

void Player::SetGameId(int id) { m_gameId = id; }
void Player::SetLobbyId(int newLobbyId) { m_lobbyId = newLobbyId; }
void Player::SetHost(bool hostStatus) { m_isHost = hostStatus; }
void Player::SetName(const std::string& playerName) { m_name = playerName; }
void Player::SetPosition(const Position& pos) { m_position = pos; }
void Player::SetDirection(const Direction& dir) { m_direction = dir; }
void Player::SetHealth(int hp) { m_health = hp; }
