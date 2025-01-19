#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <string>
#include <vector>
#include <qobject.h>
#include <crow/json.h>
#include "Player.h"
#include "InputHandler.h"
#include "EndGameWindow.h"
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
struct PlayerData {
    int health;
    Position position;
    Direction direction;
    int monkeyType;
    int isAlive;
};

class GameWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameWindow(Player& player, QWidget* parent = nullptr);
    ~GameWindow();

public slots:
    void HandleEndGameWindowClosed();

private:
    // Core Components
    Player& m_player;                     // Reference to the Player object
    InputHandler m_playerInput;           // Handles input events like movement and shooting
    std::vector<std::vector<int>> m_map;  // Local copy of the map (2D grid representation)
    std::vector<std::pair<Position, Direction>> m_bulletsCoordinates; // Bullets with position and direction
    std::unordered_map<std::string, PlayerData> m_playersData; // Map of player name to their data
    QTimer* m_timer;                      // Timer for the game loop
    ix::WebSocket m_webSocket;             // Socket for communications
    QString m_basePath = QCoreApplication::applicationDirPath() + "/Images/";
    EndGameWindow* m_endGameWindow;
    QMap<int, QPixmap> m_monkeyTextures;
    QMap<int, QPixmap> m_textures;
    //banana rendering
    QPixmap banana;
    float m_bulletRotationAngle = 0.0f;
    bool m_gameOver = { false };
    // Core Game Loop Methods
    void FetchArena();                  // Fetch the whole arena from the server
    void LoadArena(const crow::json::rvalue& arenaData);
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
    void LoadTextures();
};

#endif // GAMEWINDOW_H