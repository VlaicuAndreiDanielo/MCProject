#pragma once
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <string>
#include <vector>
#include <crow/json.h>
#include <ixwebsocket/IXWebSocket.h>
#include "Player.h"
#include "InputHandler.h"
#include <ixwebsocket/IXNetSystem.h>
struct PlayerData {
    int health;
    Position position;
    Direction direction;
};

class GameWindow : public QWidget {
public:
    explicit GameWindow(Player& player, QWidget* parent = nullptr);
    ~GameWindow();

    
private:
    // Core Components
    Player& m_player;                     // Reference to the Player object
    InputHandler m_playerInput;           // Handles input events like movement and shooting
    std::vector<std::vector<int>> m_map;  // Local copy of the map (2D grid representation)
    std::vector<std::pair<Position, Direction>> m_bulletsCoordinates; // Bullets with position and direction
    std::unordered_map<std::string, PlayerData> m_playersData; // Map of player name to their data
    QTimer* m_timer;                      // Timer for the game loop
    ix::WebSocket webSocket;             // Socket for communications
    // Core Game Loop Methods
    void FetchArena();                  // Fetch the whole arena from the server
    void LoadArena(const crow::json::rvalue& arenaData);
    void FetchGameState();              // Fetch game state from the server
    void UpdateGameState(const crow::json::rvalue& jsonResponse); // Orchestrates the update logic
    void SendInputToServer();           // Sends player input (movement and shooting) to the server
    void paintEvent(QPaintEvent* event) override; // Render the game window

    // Helper Methods for Game State Updates
    void HandleGameOver();
    void UpdatePlayerState(const crow::json::rvalue& playerData);
    void UpdateOtherPlayers(const crow::json::rvalue& playerData);
    void ProcessBullets(const crow::json::rvalue& playerData);
    void ProcessMapChanges(const crow::json::rvalue& mapChanges);

    //websocket functions
    void startConnection();
    void sendMessage(const std::string& message);
    void closeConnection();

    // Map Handling
    void DestroyMapWall(int x, int y);

};