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

    startConnection();

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, [this]() {
        FetchGameState();  // Fetch game state from the server
        SendInputToServer(); // Send player input to the server
        update();  // Trigger repaint
        });

    m_timer->start(TimingConfig::kGameLoopIntervalMs);
}

GameWindow::~GameWindow()
{
    closeConnection();
    ix::uninitNetSystem();
}

void GameWindow::startConnection() {
    ix::initNetSystem();
    // Set the URL to the WebSocket server you are trying to connect to
    webSocket.setUrl("ws://localhost:8080/webSocket");

    // Set up the 'on message' callback
    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                std::cout << msg->str << std::endl;
            }
        }
    );


    webSocket.start();
}



void GameWindow::sendMessage(const std::string& message)
{
   webSocket.send(message);
}

void GameWindow::closeConnection()
{
    webSocket.stop();
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
            update();
        }
    }
}

void GameWindow::LoadArena(const crow::json::rvalue& arenaData) {
    m_map.clear();
    for (const auto& row : arenaData["arena"]) {
        std::vector<int> mapRow;
        for (const auto& tile : row) {
            mapRow.push_back(tile.i());
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

    if (jsonResponse["isGameOver"])
        HandleGameOver();

    m_bulletsCoordinates.clear();

    for (const auto& playerData : jsonResponse["players"]) {
        int id = playerData["id"].i();

        if (id == m_player.GetId())
            UpdatePlayerState(playerData);
        else
            UpdateOtherPlayers(playerData);

        ProcessBullets(playerData);
    }

    ProcessMapChanges(jsonResponse["mapChanges"]);
}


void GameWindow::SendInputToServer() {
    // Movement payload
   /* std::string payload = R"({
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

    }*/ 
    // will replace /is_shooting and /player_move with /webSocket
    //will send a json w player movement and rotation at the same time.
    sendMessage("works");
}

void GameWindow::DestroyMapWall(int x, int y) {
    if (x >= 0 && x < m_map[0].size() && y >= 0 && y < m_map.size()) {
        m_map[y][x] = 0; //TODO replace with the type of tile the destroyed wall becomes and don't use hardcoded numbers
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

    // Set pen for text
    painter.setPen(Qt::white);

    // Draw local player's name above the rectangle
    painter.drawText(-RenderConfig::kPlayerSize / 2, -RenderConfig::kPlayerSize - 5, QString::fromStdString(m_player.GetName()));

    // Draw local player's health below the rectangle
    painter.drawText(-RenderConfig::kPlayerSize / 2, RenderConfig::kPlayerSize + 15, QString("HP: %1").arg(m_player.GetHealth()));

    painter.restore();


    // Draw other players
    for (const auto& [name, data] : m_playersData) {
        const auto& [health, position, direction] = data;

        int x = position.first - m_player.GetPosition().first + (width() / 2);
        int y = position.second - m_player.GetPosition().second + (height() / 2);

        // Draw player rectangle
        QRect playerRect(x - RenderConfig::kPlayerSize / 2, y - RenderConfig::kPlayerSize / 2, RenderConfig::kPlayerSize, RenderConfig::kPlayerSize);
        painter.setBrush(Qt::green);
        painter.drawRect(playerRect);

        // Draw player's name above the rectangle
        painter.setPen(Qt::white);
        painter.drawText(x - RenderConfig::kPlayerSize / 2, y - RenderConfig::kPlayerSize - 5, QString::fromStdString(name));

        // Draw player's health below the rectangle
        painter.drawText(x - RenderConfig::kPlayerSize / 2, y + RenderConfig::kPlayerSize + 15, QString("HP: %1").arg(health));
    }

    // Draw bullets
    painter.setBrush(Qt::red);
    for (const auto& [position, direction] : m_bulletsCoordinates) {
        QRect bulletRect(position.first - RenderConfig::kBulletSize / 2, position.second - RenderConfig::kBulletSize / 2, RenderConfig::kBulletSize, RenderConfig::kBulletSize);
        painter.drawEllipse(bulletRect);
    }
}


void GameWindow::HandleGameOver()
{
    //TODO implement what happens for gameover. I think it should transision to another window
    return;
}

void GameWindow::UpdatePlayerState(const crow::json::rvalue& playerData) {
    Position position = { playerData["x"].d(), playerData["y"].d() };
    Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };

    m_player.SetPosition(position);
    m_player.SetDirection(direction);
    m_player.SetHealth(playerData["hp"].i());

    if (playerData["hp"].i() <= 0) {
        // TODO: Handle player death
    }
}

void GameWindow::UpdateOtherPlayers(const crow::json::rvalue& playerData) {
    std::string name = playerData["name"].s();
    Position position = { playerData["x"].d(), playerData["y"].d() };
    Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };
    int health = playerData["hp"].i();

    if (health <= 0) {
        //TODO Handle other players death
    }

    m_playersData[name] = { health, position, direction };
}



void GameWindow::ProcessBullets(const crow::json::rvalue& playerData) {
    for (const auto& bullet : playerData["weapon"]["bullets"]) {
        Position bulletPosition = { bullet["x"].d(), bullet["y"].d() };
        Direction bulletDirection = { bullet["directionX"].d(), bullet["directionY"].d() };
        m_bulletsCoordinates.push_back({ bulletPosition, bulletDirection });
    }
}

void GameWindow::ProcessMapChanges(const crow::json::rvalue& mapChanges) {
    for (const auto& change : mapChanges) {
        int x = change["x"].i();
        int y = change["y"].i();
        DestroyMapWall(x, y);
    }
}
