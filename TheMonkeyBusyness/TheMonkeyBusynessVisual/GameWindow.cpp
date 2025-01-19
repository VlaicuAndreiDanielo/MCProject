#include "GameWindow.h"
#include "EndGameWindow.h"
#include <QPainter>
#include <iostream>
#include <fstream>
#include <QtWidgets/qmessagebox.h>
#include <thread>
#include <QtWidgets/QApplication>
GameWindow::GameWindow(Player& player, QWidget* parent)
    : QWidget(parent), m_player(player), m_playerInput(this) {
    LoadTextures();
    FetchArena(); // Fetch the entire arena

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    this->installEventFilter(&m_playerInput); // Install InputHandler as the event filter

    resize(RenderConfig::kWindowWidth, RenderConfig::kWindowHeight);

    startConnection();

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, [this]() {
        SendInputToServer(); 
        m_bulletRotationAngle += 5.0f; 
        if (m_bulletRotationAngle >= 360.0f) {
            m_bulletRotationAngle -= 360.0f;
        }
        update();  
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
        m_webSocket.setUrl(m_player.GetServerUrl() + "/webSocket");
       // m_webSocket.disableAutomaticReconnection();
        // Set up the 'on message' callback
        m_webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& response)
            {
                if (response->type == ix::WebSocketMessageType::Message)
                {
                    //Extract the payload (JSON string) from the WebSocket response
                    const std::string& jsonPayload = response->str;

                    // Parse the JSON payload using crow::json::load
                    auto jsonResponse = crow::json::load(jsonPayload);


                    UpdateGameState(jsonResponse);

                }
                else if (response->type == ix::WebSocketMessageType::Error)
                {
                    std::cerr << "WebSocket error: " << response->errorInfo.reason << std::endl;
                }
            }
        ); 
        m_webSocket.start();
}



void GameWindow::sendMessage(const std::string& message)
{
   m_webSocket.send(message);
}

void GameWindow::closeConnection()
{

    m_webSocket.close();
    ix::uninitNetSystem();
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



void GameWindow::UpdateGameState(const crow::json::rvalue& jsonResponse) {

    if (jsonResponse["isGameOver"].b())
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
 

    std::string payload = R"({
        "playerId":)" + std::to_string(m_player.GetId()) + R"(,
        "gameId":)" + std::to_string(m_player.GetGameId()) + R"(,
        "deltaX":)" + std::to_string(m_playerInput.m_direction.x()) + R"(,
        "deltaY":)" + std::to_string(m_playerInput.m_direction.y()) + R"(,
        "is_shooting":)" + std::to_string((int)m_playerInput.is_shooting) + R"(,
        "is_specialAblity":)" + std::to_string((int)m_playerInput.is_specialAbility) + R"(,
        "width":)" + std::to_string(width()) + R"(,
        "height":)" + std::to_string(height()) + R"(,
        "mouseX":)" + std::to_string(m_playerInput.m_mousePosition.x()) + R"(,
        "mouseY":)" + std::to_string(m_playerInput.m_mousePosition.y()) + R"(})";
        
    sendMessage(payload);
}

void GameWindow::DestroyMapWall(int x, int y) {
    if (x >= 0 && x < m_map[0].size() && y >= 0 && y < m_map.size()) {
        m_map[y][x] = 0; //TODO replace with the type of tile the destroyed wall becomes and don't use hardcoded numbers
    }
}

void GameWindow::LoadTextures()
{
    m_textures[0] = QPixmap(m_basePath + "grass.png");           // Empty
    m_textures[1] = QPixmap(m_basePath + "grass.png");           // Spawn
    m_textures[2] = QPixmap(m_basePath + "brick.png");  // Indestructible Wall
    m_textures[3] = QPixmap(m_basePath + "crate.png");    // Destructible Wall
    m_textures[4] = QPixmap(m_basePath + "water.png");           // Water
    m_textures[5] = QPixmap(m_basePath + "grassDark.png");           // Grass
    m_textures[6] = QPixmap(m_basePath + "lava.png");            // Lava
    m_textures[7] = QPixmap(m_basePath + "grass.png");      // Teleporter
    m_textures[8] = QPixmap(m_basePath + "crate.png");
    banana = QPixmap(m_basePath + "banana.png");

    m_monkeyTextures[0] = QPixmap(m_basePath + "monke.png");           
    m_monkeyTextures[1] = QPixmap(m_basePath + "capucino.png");
    m_monkeyTextures[2] = QPixmap(m_basePath + "gorilla.png");
    m_monkeyTextures[3] = QPixmap(m_basePath + "orangutan.png");

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
    std::unordered_map<std::string, PlayerData>::iterator iterator;
    if (!m_playersData.empty()) {
        iterator = m_playersData.begin();
    }
    else {
        if (!m_player.GetisAlive()) {
            return;
        }
    }
  

    if (!m_player.GetisAlive()) 
    {
        if (iterator != m_playersData.end()) {
            const auto& [name, data] = *iterator;
            if (!data.isAlive) {
                iterator++;
            }
        }
    }

    // Center the viewport on the player
    if (m_player.GetisAlive()) {
        QPointF playerOffset(width() / 2.0 - m_player.GetPosition().first,
            height() / 2.0 - m_player.GetPosition().second);
        painter.translate(playerOffset);
    }
    else {
        const auto& [name, data] = *iterator;
        const auto& [health, position, direction, monkeyType, isAlive] = data;
        QPointF playerOffset(width() / 2.0 - position.first,
            height() / 2.0 - position.second);
        painter.translate(playerOffset);
    }

    // Draw the map 
    for (int i = 0; i < m_map.size(); ++i) {
        for (int j = 0; j < m_map[i].size(); ++j) {
            QRect square(j * RenderConfig::kTileSize, i * RenderConfig::kTileSize,
                RenderConfig::kTileSize, RenderConfig::kTileSize);
            int tileType = m_map[i][j];

            if (m_textures.contains(tileType) && !m_textures[tileType].isNull()) {
                painter.drawPixmap(square, m_textures[tileType].scaled(RenderConfig::kTileSize, RenderConfig::kTileSize));
            }
            else {
                // Fallback for missing texture
                painter.fillRect(square, Qt::black);
            }

            // Draw the grid lines (optional)
            //painter.drawRect(square);
        }
    }

    painter.save();

    if (m_player.GetisAlive()) {
        // Draw the local player
        QPixmap monkeyTexture = m_monkeyTextures[m_player.GetMonkeyType()];
        QRect playerRect(
            -RenderConfig::kPlayerSize / 2,
            -RenderConfig::kPlayerSize / 2,
            RenderConfig::kPlayerSize, RenderConfig::kPlayerSize
        );

        painter.translate(m_player.GetPosition().first, m_player.GetPosition().second);
        painter.rotate(CalculateAngle(m_player.GetDirection()));
        painter.drawPixmap(playerRect, monkeyTexture);


        painter.restore();

        // Set pen for text
        painter.setPen(Qt::white);

        // Draw local player's name above the rectangle
        painter.drawText(m_player.GetPosition().first - RenderConfig::kPlayerSize / 2, m_player.GetPosition().second + RenderConfig::kPlayerSize + 5, QString::fromStdString(m_player.GetName()));

        // Draw local player's health below the rectangle
        painter.drawText(m_player.GetPosition().first - RenderConfig::kPlayerSize / 2, m_player.GetPosition().second + RenderConfig::kPlayerSize + 15, QString("HP: %1").arg(m_player.GetHealth()));
    }
    else {
        const auto& [name, data] = *iterator;
        const auto& [health, position, direction, monkeyType, isAlive] = data;
        QRect playerRect(
            -RenderConfig::kPlayerSize / 2,
            -RenderConfig::kPlayerSize / 2,
            RenderConfig::kPlayerSize, RenderConfig::kPlayerSize
        );
        QPixmap monkeyTexture = m_monkeyTextures[monkeyType];
        painter.translate(position.first, position.second);
        painter.rotate(CalculateAngle(direction));
        painter.drawPixmap(playerRect, monkeyTexture);


        painter.restore();

        // Set pen for text
        painter.setPen(Qt::white);

        painter.drawText(position.first - RenderConfig::kPlayerSize / 2, position.second + RenderConfig::kPlayerSize + 5, QString::fromStdString(name));

        // Draw player's health below the rectangle
        painter.drawText(position.first - RenderConfig::kPlayerSize / 2, position.second + RenderConfig::kPlayerSize + 15, QString("HP: %1").arg(health));
        
       
    }
    // Draw other players
    for (const auto& [name, data] : m_playersData) {
        const auto& [health, position, direction, monkeyType, isAlive] = data;

        /*int x = position.first - m_player.GetPosition().first + (width() / 2);
        int y = position.second - m_player.GetPosition().second + (height() / 2);*/

        // Draw player rectangle
        if (isAlive)
        {
            QRect playerRect(-RenderConfig::kPlayerSize / 2, -RenderConfig::kPlayerSize / 2, RenderConfig::kPlayerSize, RenderConfig::kPlayerSize);

            painter.save();
            QPixmap monkeyTexture = m_monkeyTextures[monkeyType];
            painter.translate(position.first, position.second);
            painter.rotate(CalculateAngle(direction));
            painter.drawPixmap(playerRect, monkeyTexture);
            painter.restore();
            // Draw player's name above the rectangle
            painter.setPen(Qt::white);
            painter.drawText(position.first - RenderConfig::kPlayerSize / 2, position.second + RenderConfig::kPlayerSize + 5, QString::fromStdString(name));

            // Draw player's health below the rectangle
            painter.drawText(position.first - RenderConfig::kPlayerSize / 2, position.second + RenderConfig::kPlayerSize + 15, QString("HP: %1").arg(health));
        }
    }



   for (const auto& [position, direction] : m_bulletsCoordinates) {
    QPointF bulletCenter(position.first, position.second);

    painter.save();

    painter.translate(bulletCenter);

    painter.rotate(m_bulletRotationAngle);

    QRect bulletRect(-RenderConfig::kBulletSize / 2, -RenderConfig::kBulletSize / 2,
                     RenderConfig::kBulletSize, RenderConfig::kBulletSize);

    painter.drawPixmap(bulletRect, banana.scaled(RenderConfig::kBulletSize, RenderConfig::kBulletSize, Qt::KeepAspectRatio));

    painter.restore();
   }

}


void GameWindow::HandleGameOver()
{
    if (!m_gameOver) {
        m_gameOver = true;
        m_timer->stop();
        closeConnection();
        m_endGameWindow = new EndGameWindow(&m_player);
        connect(m_endGameWindow, &EndGameWindow::EndGameWindowClosed, this, &GameWindow::HandleEndGameWindowClosed);

        m_endGameWindow->show();

        //close();
    }
    return;
}

void GameWindow::HandleEndGameWindowClosed(){
}

void GameWindow::UpdatePlayerState(const crow::json::rvalue& playerData) {
    Position position = { playerData["x"].d(), playerData["y"].d() };
    Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };

    m_player.SetPosition(position);
    m_player.SetDirection(direction);
    m_player.SetHealth(playerData["hp"].i());
    m_player.SetMonkey(playerData["monkeyType"].i());
    m_player.SetisAlive(playerData["isAlive"].i());
}

void GameWindow::UpdateOtherPlayers(const crow::json::rvalue& playerData) {
    std::string name = playerData["name"].s();
    Position position = { playerData["x"].d(), playerData["y"].d() };
    Direction direction = { playerData["directionX"].d(), playerData["directionY"].d() };
    int health = playerData["hp"].i();
    int monkeyType = playerData["monkeyType"].i();
    int isAlive = playerData["isAlive"].i();
    m_playersData[name] = { health, position, direction, monkeyType, isAlive};
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
