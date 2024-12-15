#pragma once
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <string>
#include <vector>
#include <crow/json.h>
#include "Player.h"
#include "InputHandler.h"

constexpr int SQUARE_SIZE = 40;
constexpr int kPlayerSize = 30;

class GameWindow : public QWidget {
    Q_OBJECT
public:
    explicit GameWindow(Player& player, QWidget* parent = nullptr);

private:
    Player& player;                     // Reference to the Player object
    InputHandler playerInput;           // Handles input events like movement and shooting
    std::vector<std::vector<int>> map;  // Local copy of the map (2D grid representation)
    std::vector<std::pair<Position, Direction>> bulletsCoordinates; // Bullets with position and direction
    std::vector<std::pair<Position, Direction>> playersCoordinates; // Other players' positions and directions
    QTimer* timer;                      // Timer for the game loop

    void fetchArena();                  // Fetch the whole arena from the server
    void loadArena(const crow::json::rvalue& arenaData);
    void fetchGameState();              // Fetch game state from the server
    void updateGameState(const crow::json::rvalue& jsonResponse); // Update local state based on server data
    void sendInputToServer();           // Sends player input (movement and shooting) to the server
    void paintEvent(QPaintEvent* event) override; // Render the game window
};
