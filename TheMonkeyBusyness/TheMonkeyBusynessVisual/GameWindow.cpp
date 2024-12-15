#include "GameWindow.h"
#include <QPainter>
#include <iostream>
#include <fstream>

GameWindow::GameWindow(Player& player, QWidget* parent)
    : QWidget(parent), player(player), playerInput(this) {
    fetchArena(); // Fetch the entire arena

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    this->installEventFilter(&playerInput); // Install InputHandler as the event filter

    resize(800, 600); // Adjust dimensions as needed

    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [this]() {
        fetchGameState();  // Fetch game state from the server
        sendInputToServer(); // Send player input to the server
        update();  // Trigger repaint
        });

    timer->start(64); // ~60 FPS
}


void GameWindow::fetchArena() {
    cpr::Response response = cpr::Get(
        cpr::Url{ player.getServerUrl() + "/game_arena" },
        cpr::Parameters{ {"gameId", std::to_string(player.getGameId())} }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse.has("arena")) {
            loadArena(jsonResponse);
            update(); // Trigger paint event
        }
        else {
            std::cerr << "Error: Arena map not found in response." << std::endl;
        }
    }
    else {
        std::cerr << "Error: Server responded with code " << response.status_code << std::endl;
    }
}

void GameWindow::loadArena(const crow::json::rvalue& arenaData) {
    map.clear();
    for (const auto& row : arenaData["arena"]) {
        std::vector<int> mapRow;
        for (const auto& tile : row) {
            mapRow.push_back(tile.i()); // Convert JSON tile to integer
        }
        map.push_back(std::move(mapRow));
    }
}

void GameWindow::fetchGameState() {
    cpr::Response response = cpr::Get(
        cpr::Url{ player.getServerUrl() + "/game_state" },
        cpr::Parameters{ {"gameId", std::to_string(player.getGameId())} }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        if (jsonResponse) {
            updateGameState(jsonResponse);
        }
        else {
            std::cerr << "Error: Failed to parse game state JSON." << std::endl;
        }
    }
    else {
        std::cerr << "Error: Server responded with code " << response.status_code << std::endl;
    }
}

void GameWindow::updateGameState(const crow::json::rvalue& jsonResponse) {
    playersCoordinates.clear();
    bulletsCoordinates.clear();

    for (const auto& playerData : jsonResponse["players"]) {
        Position position = { playerData["x"].d(), playerData["y"].d() };
        Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };
        int id = playerData["id"].i();

        if (id == player.getId()) {
            // Update local player's attributes
            player.setPosition(position);
            player.setDirection(direction);
            player.setHealth(playerData["hp"].i());
        }
        else {
            // Update other players
            playersCoordinates.push_back({ position, direction });
        }

        // Process bullets for this player
        for (const auto& bullet : playerData["weapon"]["bullets"]) {
            Position bulletPosition = { bullet["x"].d(), bullet["y"].d() };
            Direction bulletDirection = { bullet["directionX"].d(), bullet["directionY"].d() };
            bulletsCoordinates.push_back({ bulletPosition, bulletDirection });
        }
    }
}


void GameWindow::sendInputToServer() {
    // Movement payload
    std::string payload = R"({
        "playerId":)" + std::to_string(player.getId()) + R"(,
        "gameId":)" + std::to_string(player.getGameId()) + R"(,
        "deltaX":)" + std::to_string(playerInput.m_direction.x()) + R"(,
        "deltaY":)" + std::to_string(playerInput.m_direction.y()) + R"(,
        "mouseX":)" + std::to_string(playerInput.m_mousePosition.x()) + R"(,
        "mouseY":)" + std::to_string(playerInput.m_mousePosition.y()) + R"(})";

    cpr::Response response = cpr::Post(
        cpr::Url{ player.getServerUrl() + "/player_move" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ payload }
    );

    if (response.status_code != 200) {
        std::cerr << "Error: Failed to send movement. Response: " << response.text << std::endl;
    }

    // Shooting payload
    if (playerInput.is_shooting) {
        std::string shootPayload = R"({
            "playerId":)" + std::to_string(player.getId()) + R"(,
            "gameId":)" + std::to_string(player.getGameId()) + R"(,
            "mouseX":)" + std::to_string(playerInput.m_mousePosition.x()) + R"(,
            "mouseY":)" + std::to_string(playerInput.m_mousePosition.y()) + R"(})";

        cpr::Response shootResponse = cpr::Post(
            cpr::Url{ player.getServerUrl() + "/shoot_weapon" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{ shootPayload }
        );

        if (shootResponse.status_code != 200) {
            std::cerr << "Error: Failed to send shoot input. Response: " << shootResponse.text << std::endl;
        }
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
    QPointF playerOffset(width() / 2.0 - player.getPosition().first,
        height() / 2.0 - player.getPosition().second);
    painter.translate(playerOffset);

    // Draw the map
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            QRect square(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
            switch (map[i][j]) {
            case 0: painter.fillRect(square, Qt::white); break;  // Empty
            case 1: painter.fillRect(square, Qt::green); break;  // Spawn
            case 2: painter.fillRect(square, Qt::darkGray); break;  // Indestructible Wall
            case 3: painter.fillRect(square, Qt::red); break;  // Destructible Wall
            default: painter.fillRect(square, Qt::black); break;  // Unknown
            }
            painter.drawRect(square);
        }
    }
    painter.save();
    // Draw the local player
    QRect playerRect(
        - kPlayerSize / 2,
        - kPlayerSize / 2,
        kPlayerSize, kPlayerSize
    );
    painter.translate(player.getPosition().first, player.getPosition().second);
    painter.rotate(CalculateAngle(player.getDirection()));
    painter.setBrush(Qt::cyan);
    painter.drawRect(playerRect);
    
    painter.restore();
    // Draw other players
    painter.setBrush(Qt::green);
    for (const auto& [position, direction] : playersCoordinates) {
        int x = position.first - player.getPosition().first + (width() / 2);
        int y = position.second - player.getPosition().second + (height() / 2);
        painter.drawRect(QRect(x - kPlayerSize / 2, y - kPlayerSize / 2, kPlayerSize, kPlayerSize));
    }

    // Draw bullets
    painter.setBrush(Qt::red);
    for (const auto& [position, direction] : bulletsCoordinates) {
        QRect bulletRect(position.first - 5, position.second - 5, 10, 10); // Center bullets
        painter.drawEllipse(bulletRect);
    }
}
