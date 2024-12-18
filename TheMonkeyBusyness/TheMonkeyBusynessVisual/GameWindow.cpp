#include "GameWindow.h"
#include <QPainter>
#include <iostream>
#include <fstream>

GameWindow::GameWindow(Player& player, QWidget* parent)
    : QWidget(parent), m_player(player), m_playerInput(this) {
    FetchArena(); // Fetch the entire arena

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    this->installEventFilter(&m_playerInput); // Install InputHandler as the event filter

    resize(RenderConfig::kWindowWidth, RenderConfig::kWindowHeight);

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, [this]() {
        FetchGameState();  // Fetch game state from the server
        SendInputToServer(); // Send player input to the server
        update();  // Trigger repaint
        });

    m_timer->start(TimingConfig::kGameLoopIntervalMs);
}


void GameWindow::FetchArena() {
    cpr::Response response = cpr::Get(
        cpr::Url{ m_player.GetServerUrl() + "/game_arena" },
        cpr::Parameters{ {"gameId", std::to_string(m_player.GetGameId())} }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse.has("arena")) {
            LoadArena(jsonResponse);
            update(); // Trigger paint event
        }
    }
}

void GameWindow::LoadArena(const crow::json::rvalue& arenaData) {
    m_map.clear();
    for (const auto& row : arenaData["arena"]) {
        std::vector<int> mapRow;
        for (const auto& tile : row) {
            mapRow.push_back(tile.i()); // Convert JSON tile to integer
        }
        m_map.push_back(std::move(mapRow));
    }
}

void GameWindow::FetchGameState() {
    cpr::Response response = cpr::Get(
        cpr::Url{ m_player.GetServerUrl() + "/game_state" },
        cpr::Parameters{ {"gameId", std::to_string(m_player.GetGameId())} }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse) {
            UpdateGameState(jsonResponse);
        }
    }
}

void GameWindow::UpdateGameState(const crow::json::rvalue& jsonResponse) {
    m_playersCoordinates.clear();
    m_bulletsCoordinates.clear();

    for (const auto& playerData : jsonResponse["players"]) {
        Position position = { playerData["x"].d(), playerData["y"].d() };
        Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };
        int id = playerData["id"].i();

        if (id == m_player.GetId()) {
            // Update local player's attributes
            m_player.SetPosition(position);
            m_player.SetDirection(direction);
            m_player.SetHealth(playerData["hp"].i());
        }
        else {
            // Update other players
            m_playersCoordinates.push_back({ position, direction });
        }

        // Process bullets for this player
        for (const auto& bullet : playerData["weapon"]["bullets"]) {
            Position bulletPosition = { bullet["x"].d(), bullet["y"].d() };
            Direction bulletDirection = { bullet["directionX"].d(), bullet["directionY"].d() };
            m_bulletsCoordinates.push_back({ bulletPosition, bulletDirection });
        }
    }
}


void GameWindow::SendInputToServer() {
    // Movement payload
    std::string payload = R"({
        "playerId":)" + std::to_string(m_player.GetId()) + R"(,
        "gameId":)" + std::to_string(m_player.GetGameId()) + R"(,
        "deltaX":)" + std::to_string(m_playerInput.m_direction.x()) + R"(,
        "deltaY":)" + std::to_string(m_playerInput.m_direction.y()) + R"(,
        "mouseX":)" + std::to_string(m_playerInput.m_mousePosition.x()) + R"(,
        "mouseY":)" + std::to_string(m_playerInput.m_mousePosition.y()) + R"(})";

    cpr::Response response = cpr::Post(
        cpr::Url{ m_player.GetServerUrl() + "/player_move" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ payload }
    );

    // Shooting payload
    if (m_playerInput.is_shooting) {
        std::string shootPayload = R"({
            "playerId":)" + std::to_string(m_player.GetId()) + R"(,
            "gameId":)" + std::to_string(m_player.GetGameId()) + R"(,
            "mouseX":)" + std::to_string(m_playerInput.m_mousePosition.x()) + R"(,
            "mouseY":)" + std::to_string(m_playerInput.m_mousePosition.y()) + R"(})";

        cpr::Response shootResponse = cpr::Post(
            cpr::Url{ m_player.GetServerUrl() + "/shoot_weapon" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{ shootPayload }
        );

    }
}

float CalculateAngle(const Direction direction) {
    float angleInRadians = atan2(direction.second, direction.first);

    // Convert radians to degrees
    float angleInDegrees = (float)(angleInRadians * (180.0f / M_PI));

    // Adjust the angle: since (0, -1) is 0 degrees, we need to shift by 90 degrees
    angleInDegrees = fmod(angleInDegrees + 90.0f, 360.0f);

    // If angle is negative, make it positive
    if (angleInDegrees < 0) {
        angleInDegrees += 360.0f;
    }

    return angleInDegrees;
}

void GameWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Center the viewport on the player
    QPointF playerOffset(width() / 2.0 - m_player.GetPosition().first,
        height() / 2.0 - m_player.GetPosition().second);
    painter.translate(playerOffset);

    // Draw the map
    for (int i = 0; i < m_map.size(); ++i) {
        for (int j = 0; j < m_map[i].size(); ++j) {
            QRect square(j * RenderConfig::kTileSize, i * RenderConfig::kTileSize, RenderConfig::kTileSize, RenderConfig::kTileSize);
            switch (m_map[i][j]) {
            case 0: painter.fillRect(square, Qt::green); break;  // Empty
            case 1: painter.fillRect(square, Qt::black); break;  // Spawn
            case 2: painter.fillRect(square, Qt::darkRed); break;  // Indestructible Wall
            case 3: painter.fillRect(square, Qt::yellow); break;  // Destructible Wall
            case 4: painter.fillRect(square, Qt::blue); break;  // Water
            case 5: painter.fillRect(square, Qt::darkGreen); break;  // Grass
            case 6: painter.fillRect(square, Qt::red); break;  // Lava
            case 7: painter.fillRect(square, Qt::darkMagenta); break;  // Teleporter
            case 8: painter.fillRect(square, Qt::darkYellow); break;  // Teleporter
            default: painter.fillRect(square, Qt::black); break;  // FakeDestructibleWall
            }
            painter.drawRect(square);
        }
    }
    painter.save();
    // Draw the local player
    QRect playerRect(
        -RenderConfig::kPlayerSize / 2,
        -RenderConfig::kPlayerSize / 2,
        RenderConfig::kPlayerSize, RenderConfig::kPlayerSize
    );

    painter.translate(m_player.GetPosition().first, m_player.GetPosition().second);
    painter.rotate(CalculateAngle(m_player.GetDirection()));
    painter.setBrush(Qt::cyan);
    painter.drawRect(playerRect);
    
    painter.restore();
    // Draw other players
    painter.setBrush(Qt::green);
    for (const auto& [position, direction] : m_playersCoordinates) {
        int x = position.first - m_player.GetPosition().first + (width() / 2);
        int y = position.second - m_player.GetPosition().second + (height() / 2);
        painter.drawRect(QRect(x - RenderConfig::kPlayerSize / 2, y - RenderConfig::kPlayerSize / 2, RenderConfig::kPlayerSize, RenderConfig::kPlayerSize));
    }

    // Draw bullets
    painter.setBrush(Qt::red);
    for (const auto& [position, direction] : m_bulletsCoordinates) {
        QRect bulletRect(position.first - RenderConfig::kBulletSize / 2, position.second - RenderConfig::kBulletSize / 2, RenderConfig::kBulletSize, RenderConfig::kBulletSize);
        painter.drawEllipse(bulletRect);
    }
}
