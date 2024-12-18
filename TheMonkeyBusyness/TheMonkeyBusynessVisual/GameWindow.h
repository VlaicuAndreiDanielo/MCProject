#pragma once
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <string>
#include <vector>
#include <crow/json.h>
#include "Player.h"
#include "InputHandler.h"

class GameWindow : public QWidget { 
    
public:
    explicit GameWindow(Player& player, QWidget* parent = nullptr);

private:
    Player& m_player;                     // Reference to the Player object
    InputHandler m_playerInput;           // Handles input events like movement and shooting
    std::vector<std::vector<int>> m_map;  // Local copy of the map (2D grid representation)
    std::vector<std::pair<Position, Direction>> m_bulletsCoordinates; // Bullets with position and direction
    std::vector<std::pair<Position, Direction>> m_playersCoordinates; // Other players' positions and directions
    QTimer* m_timer;                      // Timer for the game loop

    void FetchArena();                  // Fetch the whole arena from the server
    void LoadArena(const crow::json::rvalue& arenaData);
    void FetchGameState();              // Fetch game state from the server
    void UpdateGameState(const crow::json::rvalue& jsonResponse); // Update local state based on server data
    void SendInputToServer();           // Sends player input (movement and shooting) to the server
    void paintEvent(QPaintEvent* event) override; // Render the game window
};
