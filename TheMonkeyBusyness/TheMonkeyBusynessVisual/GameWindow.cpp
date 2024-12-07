#include "GameWindow.h"
#include <QPainter>
#include <iostream>

constexpr int SQUARE_SIZE = 40;
constexpr int kPlayerSize = 30;


GameWindow::GameWindow(int playerId, int gameId, const std::string& serverUrl, QWidget* parent)
    : QWidget(parent), playerId(playerId), gameId(gameId), serverUrl(serverUrl), playerPosition(400, 300) {

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    this->installEventFilter(&playerInput);


    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [this]() {
        fetchGameState();
        //updatePlayerDirection();
        sendPlayerInputToServer();
        update();
        });

    timer->start(32); // 16ms for ~60 FPS
}

void GameWindow::fetchGameState() {
    cpr::Response response = cpr::Get(cpr::Url{ serverUrl + "/game_state" },
        cpr::Parameters{ {"gameId", std::to_string(gameId)} });

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
    // Update players
    for (const auto& player : jsonResponse["players"]) {
        float x = player["x"].d();
        float y = player["y"].d();
        int id = player["id"].i(); 

        bulletsCoordinates.clear();

        std::ofstream logFile("client_log.txt", std::ios::app);

        for (const auto& bullet : player["weapon"]["bullets"])
        {
            Position position(bullet["x"].d(), bullet["y"].d());
            Direction direction(bullet["directionX"].d(), bullet["directionY"].d());

            std::ofstream logFile("client_log.txt", std::ios::app);
                float bulletX = bullet["x"].d();
                float bulletY = bullet["y"].d();
                float directionX = bullet["directionX"].d();
                float directionY = bullet["directionY"].d();
                logFile << "Bullet received at position: (" << bulletX << ", " << bulletY
                    << ") with direction: (" << directionX << ", " << directionY << ")" << std::endl;


            bulletsCoordinates.push_back({ position, direction });
        }

        if (id == playerId) {
            playerPosition.setX(x);
            playerPosition.setY(y);
            health = player["hp"].i();
            static int healthDamage = 1;  //TODO remove this ,temporary just for testing
            health -= healthDamage++; //TODO remove this ,temporary just for testing
        }
        else
        {
            playersCoordinates.push_back({ x, y });
        }
    }

    // Update map
    if (jsonResponse.has("arena") && jsonResponse["arena"].has("map")) {
        map.clear();
        for (const auto& row : jsonResponse["arena"]["map"]) {
            std::vector<int> mapRow;
            for (const auto& tile : row) {
                mapRow.push_back(tile.i()); // Convert JSON tile to integer
            }
            map.push_back(std::move(mapRow));
        }
    }

}

void GameWindow::updatePlayerDirection() {
    if (playerInput.m_direction.x() != 0 || playerInput.m_direction.y() != 0) {
        playerPosition.setX(playerPosition.x() + playerInput.m_direction.x() * 5);
        playerPosition.setY(playerPosition.y() + playerInput.m_direction.y() * 5);
    }
}
void GameWindow::sendPlayerInputToServer() {
    // Player Movement
    std::string payload = R"({"playerId":)" + std::to_string(playerId) +
        R"(,"gameId":)" + std::to_string(gameId) +
        R"(,"deltaX":)" + std::to_string(playerInput.m_direction.x()) +
        R"(,"deltaY":)" + std::to_string(playerInput.m_direction.y()) + "}";

    // Log the payload
    std::ofstream logFile("client_log.txt", std::ios::app);
    logFile << "Sending payload to server: " << payload << std::endl;

    cpr::Response moveResponse = cpr::Post(
        cpr::Url{ serverUrl + "/player_move" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ payload }
    );

    // Log the server's response
    logFile << "Server response: " << moveResponse.status_code
        << " - " << moveResponse.text << std::endl;

    if (moveResponse.status_code != 200) {
        logFile << "Movement failed. Payload: " << payload << std::endl;
    }

    if (playerInput.is_shooting) {
        // Construct the request body with the mouse position
        std::string payload = R"({
        "playerId":)" + std::to_string(playerId) + R"(,
        "gameId":)" + std::to_string(gameId) + R"(,
        "mouseX":)" + std::to_string(playerInput.m_mousePosition.x()) + R"(,
        "mouseY":)" + std::to_string(playerInput.m_mousePosition.y()) + R"(})";

        // Send the request
        cpr::Response shootResponse = cpr::Post(
            cpr::Url{ serverUrl + "/shoot_weapon" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{ payload }
        );

        // Log the request and response for debugging
        std::ofstream logFile("client_log.txt", std::ios::app);
        logFile << "Shooting payload: " << payload << std::endl;
        logFile << "Server response: " << shootResponse.status_code << " - " << shootResponse.text << std::endl;
    }
}


void GameWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Center the viewport on the player
    QPointF playerOffset(width() / 2.0 - playerPosition.x(), height() / 2.0 - playerPosition.y());
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
            case 4: painter.fillRect(square, Qt::blue); break;  // Water
            case 5: painter.fillRect(square, Qt::darkGreen); break;  // Grass
            case 6: painter.fillRect(square, Qt::yellow); break;  // Lava
            case 7: painter.fillRect(square, Qt::magenta); break;  // Teleporter
            default: painter.fillRect(square, Qt::black); break;  // Unknown
            }
            painter.drawRect(square);
        }
    }

    // Draw the player
    QRect playerRect(
        playerPosition.x() - kPlayerSize / 2,
        playerPosition.y() - kPlayerSize / 2,
        kPlayerSize, kPlayerSize
    );
    painter.setBrush(Qt::cyan); // Player color
    painter.drawEllipse(playerRect);

    // Draw other players
    painter.setBrush(Qt::green);
    for (const auto& position : playersCoordinates) {
        int x = position.first - playerPosition.x() + (width() / 2);
        int y = position.second - playerPosition.y() + (height() / 2);
        painter.drawEllipse(QRect(x - kPlayerSize / 2, y - kPlayerSize / 2, kPlayerSize, kPlayerSize));
    }

    // Draw bullets
    painter.setBrush(Qt::red);
    for (const auto& [position, direction] : bulletsCoordinates) {
        QRect bulletRect(position.first - 5, position.second - 5, 10, 10); // Center bullets and set size
        painter.drawEllipse(bulletRect);
    }

    // Reset painter transformations for UI elements
    painter.resetTransform();

    //// Draw health bar
    //QRect healthBar(10, 10, 200, 20); // Health bar position and size
    //painter.setBrush(Qt::red);
    //painter.drawRect(healthBar);

    //QRect healthFill(10, 10, (health / 100.0) * 200, 20); // Fill proportional to health
    //painter.setBrush(Qt::green);
    //painter.drawRect(healthFill);

    //// Draw health text
    //painter.setPen(Qt::black);
    //painter.drawText(healthBar, Qt::AlignCenter, QString("Health: %1").arg(health));

    //// Draw FPS
    //frameCount += 10;

    //if (fpsTimer.elapsed() >= 1000) { // 1000 ms = 1 second
    //    currentFPS = frameCount;
    //    frameCount = 0;
    //    fpsTimer.restart();
    //}

    //painter.setPen(Qt::black);
    //painter.setFont(QFont("Arial", 16));
    //painter.drawText(10, 40, QString("FPS: %1").arg(currentFPS));
}
