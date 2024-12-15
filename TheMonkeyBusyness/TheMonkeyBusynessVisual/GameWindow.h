#pragma once
#include <QtWidgets/QWidget>
#include <QtCore/QPoint>
#include <QtCore/QTimer>
#include <QElapsedTimer>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <crow/json.h>
#include "InputHandler.h"

using Position = std::pair<float, float>;
using Direction = std::pair<float, float>;

class GameWindow : public QWidget {
    //Q_OBJECT;
public:
    explicit GameWindow(int playerId, int gameId, const std::string& serverUrl, QWidget* parent = nullptr);

private:
    int playerId;                      // Player's unique ID
    int gameId;                        // Game's unique ID
    std::string serverUrl;             // Server URL for requests
    Position playerPosition;           // Current position of the player
    Direction playerDirection;         // Current direction of the player
    InputHandler playerInput;          // Handles user inputs (movement, shooting)
    QTimer* timer;                     // Timer for the game loop

    std::vector<std::vector<int>> map; // Local copy of the map (2D grid representation)
    std::vector<std::pair<Position, Direction>> bulletsCoordinates; // Store bullets with position and direction
    int health;
    std::vector<std::pair<Position, Direction>> playersCoordinates; // Store positions and directions for all players

    void fetchArena();  // Fetch the whole arena from the server
    void loadArena(const crow::json::rvalue& arenaData);
    void fetchGameState();             // Fetch game state from the server
    void updateGameState(const crow::json::rvalue& jsonResponse); // Update local state based on server data
    void updatePlayerDirection();      // Update player movement locally
    void sendPlayerInputToServer();    // Send input data (movement, shooting) to the server

    void paintEvent(QPaintEvent* event) override; // Render the game window
};
