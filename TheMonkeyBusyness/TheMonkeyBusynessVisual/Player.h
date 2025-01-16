#pragma once
#include <string>
#include <utility>
#include <cpr/cpr.h>
#include <crow/json.h>
#include "Constants.h"

class Player {
public:
    Player(int id, const std::string& serverUrl);

    // Static method for logging in
    static int LogIn(const std::string& serverUrl, const std::string& username, const std::string& password);
    static int SignIn(const std::string& serverUrl, const std::string& username, const std::string& password);
    // Lobby-related methods
    int CreateLobby();
    bool JoinLobby(int lobbyId);
    bool LeaveLobby();
    bool SetReady();
    crow::json::wvalue GetActiveLobbies();
    crow::json::wvalue GetLobbyDetails(int lobbyId);

    // Game-related methods
    int StartGame();

    // Accessors
    int GetId() const;
    int GetGameId() const;
    int GetLobbyId() const;
    bool GetIsReady() const;
    bool GetIsHost() const;
    std::string GetName() const;
    Position GetPosition() const;
    Direction GetDirection() const;
    const std::string& GetServerUrl() const;
    int GetHealth() const;
    int GetMonkeyType()const;

    // Mutators
    void SetGameId(int id);
    void SetLobbyId(int lobbyId);
    void SetHost(bool hostStatus);
    void SetName(const std::string& playerName);
    void SetPosition(const Position& position);
    void SetDirection(const Direction& direction);
    void SetHealth(int health);
    void SetMonkey(int monkeyType);

private:
    int m_id;                            // Player's unique ID
    int m_gameId;                        // The game ID the player is participating in
    int m_lobbyId;                       // The lobby ID the player is in
    int m_health;                        // Player's health
    int m_monkeyType;                    // Player's monkey type
    bool m_isReady;                      // If player in the lobby is ready to start the game or not
    bool m_isHost;                       // If the player is the host of the lobby
    std::string m_name;                  // Player's name
    std::string m_serverUrl;             // Server URL for requests
    Position m_position;                 // Player's current position
    Direction m_direction;               // Player's current direction
};
