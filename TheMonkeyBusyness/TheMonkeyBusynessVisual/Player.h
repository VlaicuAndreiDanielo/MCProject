#pragma once
#include <string>
#include <utility> // for std::pair
#include <cpr/cpr.h>
#include <crow/json.h>

using Position = std::pair<float, float>;
using Direction = std::pair<float, float>;

class Player {
public:
    Player(int id, const std::string& serverUrl);

    // Static methods for login and game creation
    static int logIn(const std::string& serverUrl, const std::string& username, const std::string& password);
    static int createGame(const std::string& serverUrl, int playerId);

    // Methods to interact with the game
    void startGame(int gameId);

    // Accessors
    int getId() const;
    int getGameId() const;
    std::string getName() const;
    Position getPosition() const;
    Direction getDirection() const;
    const std::string& getServerUrl() const;


    // Mutators
    void setGameId(int id);
    void setName(const std::string& playerName);
    void setPosition(const Position& position);
    void setDirection(const Direction& direction);
    void setHealth(int health);


private:
    int id;                            // Player's unique ID
    int gameId;  // The game ID the player is participating in
    std::string name; // Player's name
    std::string serverUrl;             // Server URL for requests
    Position position;                 // Player's current position
    Direction direction;               // Player's current direction
    int health;                        // Player's health
};
