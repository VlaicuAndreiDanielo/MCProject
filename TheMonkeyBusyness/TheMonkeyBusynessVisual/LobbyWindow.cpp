/*Versiunea cu QListWidget pt lobbyuri*/
#include "LobbyWindow.h"
#include "SessionManager.h"
#include "GameWindow.h"
#include "Player.h"
#include <QtWidgets/QGraphicsDropShadowEffect> 
#include <QtCore/QCoreApplication>             
#include <QtGui/QPixmap>                       
#include <QtGui/QPalette>                      
#include <QtWidgets/QMessageBox> 


LobbyWindow::LobbyWindow(int playerId, QWidget* parent) :m_playerId(playerId), QWidget(parent) {
    m_player = new Player(playerId, serverUrl);
    //webSocket = new ix::WebSocket();
    SetupUI();
    GetLobbiesFromServer();
    //StartConnectionWebSocket();

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, [this]() {
       /* std::string payload = R"({
        "playerId":)" + std::to_string(m_player->GetId()) + R"(})";*/
       // SendMessageWebSocket(payload);
        CheckStart();
    });

    m_timer->start(TimingConfig::kGameLoopIntervalMs);
}

LobbyWindow::~LobbyWindow()
{
    //CloseConnectionWebSocket();
}

//void LobbyWindow::StartConnectionWebSocket()
//{
//    ix::initNetSystem();
//    // Set the URL to the WebSocket server you are trying to connect to
//    webSocket->setUrl(m_player->GetServerUrl() + "/lobbysocket");
//
//    // Set up the 'on message' callback
//    webSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& response)
//    {
//            if (response->type == ix::WebSocketMessageType::Message)
//            {
//                // Extract the payload (JSON string) from the WebSocket response
//                const std::string& jsonPayload = response->str;
//                // Parse the JSON payload using crow::json::load
//                auto jsonResponse = crow::json::load(jsonPayload);
//
//                int startGame = jsonResponse["startCheck"].i();
//                if (startGame == 1) {
//                    if (!startedGame) {
//                        startedGame = true;
//                        m_player->SetGameId(jsonResponse["gameId"].i());
//                        GameWindow* gameWindow = new GameWindow(*m_player);
//                        gameWindow->show();
//                        //CloseConnectionWebSocket();
//                        //emit LobbyWindowClosed(); // Emit semnalul când se apasă Quit
//                        close();
//                    }
//                }
//
//            }
//            else if (response->type == ix::WebSocketMessageType::Error)
//            {
//                std::cerr << "WebSocket error: " << response->errorInfo.reason << std::endl;
//            }
//    });
//
//    webSocket->start();
//}

//void LobbyWindow::SendMessageWebSocket(const std::string& message)
//{
//    webSocket->send(message);
//}
//
//void LobbyWindow::CloseConnectionWebSocket()
//{
//    if (webSocket->getReadyState() == ix::ReadyState::Open || webSocket->getReadyState() == ix::ReadyState::Connecting) {
//        static bool connectionClosed = false; // Prevent double-close
//        if (!connectionClosed) {
//            connectionClosed = true;
//            webSocket->stop();
//        }
//    }
//}

void LobbyWindow::CheckStart()
{
    cpr::Response response = cpr::Get(
        cpr::Url{ m_player->GetServerUrl() + "/startgameclient"},
        cpr::Parameters{ {"playerId", std::to_string(m_player->GetId())} }
    );

    if (response.status_code == 200) {
        auto jsonResponse = crow::json::load(response.text);
        int startGame = jsonResponse["startCheck"].i();
        if (startGame == 1) {
            if (!startedGame && !host) {
                startedGame = true;
                m_player->SetGameId(jsonResponse["gameId"].i());
                m_timer->stop();
                GameWindow* gameWindow = new GameWindow(*m_player);
                gameWindow->show();
                close();
            }
        }
    }
}

void LobbyWindow::SetupUI() {
    setWindowTitle("Lobby");
    showFullScreen();

    // **Setăm imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background_playwindow.jpg";
    QPixmap background(imagePath);
    if (!background.isNull()) {
        background = background.scaled(size(), Qt::KeepAspectRatioByExpanding);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background));
        setPalette(palette);
    }

    // Layout principal pe orizontală
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // **Primul VerticalBox - Titlul și butoanele**
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(20);

    // Titlul
    m_titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 56, QFont::Bold);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: rgba(255, 223, 0, 255);
            qproperty-alignment: AlignCenter;
            word-wrap: true; /* Permite spargerea textului pe mai multe linii */
        }
    )");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setWordWrap(true); // Activează împachetarea textului

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    m_titleLabel->setGraphicsEffect(shadowEffect);

    leftLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);

    // Buton Play
    m_playButton = new QPushButton("Start Game", this);
    m_playButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 215, 0, 200);
            color: black;
            font-size: 24px;
            font-weight: bold;
            border-radius: 15px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: rgba(255, 200, 0, 230);
        }
    )");
    m_playButton->setFixedSize(200, 60);
    leftLayout->addWidget(m_playButton, 0, Qt::AlignCenter);

    // Buton Create Lobby
    m_createLobbyButton = new QPushButton("Create Lobby", this);
    m_createLobbyButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 215, 0, 200);
            color: black;
            font-size: 24px;
            font-weight: bold;
            border-radius: 15px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: rgba(255, 200, 0, 230);
        }
    )");
    m_createLobbyButton->setFixedSize(200, 60);
    leftLayout->addWidget(m_createLobbyButton, 0, Qt::AlignCenter);

    // Spațiu flexibil pentru butonul Quit
    leftLayout->addStretch();

    // Buton Quit
    m_quitButton = new QPushButton("Quit", this);
    m_quitButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 100, 100, 200);
            color: white;
            font-size: 20px;
            font-weight: bold;
            border-radius: 15px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: rgba(255, 70, 70, 230);
        }
    )");
    m_quitButton->setFixedSize(150, 50);
    leftLayout->addWidget(m_quitButton, 0, Qt::AlignCenter);

    // **Al doilea VerticalBox - Lista de lobby-uri**
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(10);

    // Lista de lobby-uri
    m_lobbyList = new QListWidget(this);
    m_lobbyList->setStyleSheet(R"(
        QListWidget {
            background-color: rgba(255, 255, 255, 200);
            border: 2px solid black;
            border-radius: 10px;
            font-size: 16px;
        }
    )");
    m_lobbyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(m_lobbyList);

    mainLayout->addLayout(leftLayout, 1); // Adaugă primul VerticalBox
    mainLayout->addLayout(rightLayout, 3); // Adaugă al doilea VerticalBox

    setLayout(mainLayout);

    // Conexiuni pentru butoane
    connect(m_playButton, &QPushButton::clicked, this, &LobbyWindow::OnPlayButtonClicked);
    connect(m_createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::OnCreateLobbyButtonClicked);
    connect(m_quitButton, &QPushButton::clicked, this, &LobbyWindow::OnQuitButtonClicked);
    connect(m_lobbyList, &QListWidget::itemClicked, this, &LobbyWindow::OnItemClicked);
}

void LobbyWindow::GetLobbiesFromServer()
{
    crow::json::wvalue lobbyJson = m_player->GetActiveLobbies();
    if (lobbyJson["lobbies"].t() != crow::json::type::List) {
        QMessageBox::information(this, "Create Lobby","No lobbies");
        return;
    }
    


    std::vector<int> lobbyIds;
    for (size_t i = 0; i < lobbyJson["lobbies"].size(); ++i) {
        try {
            std::string id = lobbyJson["lobbies"][i].dump();
            lobbyIds.push_back(std::stoi(id));  
        }
        catch (const std::exception& e) {
            QMessageBox::information(this, "Create Lobby", "Error converting lobby ID");
        }
    }
    
    GetLobbyData(lobbyIds);

    // TO DO: Get all lobbies from server and display them on all clients over here
}

void LobbyWindow::GetLobbyData(std::vector<int> lobbyIds)
{
    for (int id : lobbyIds) {
        GetLobbyData(id);
    }
}

void LobbyWindow::GetLobbyData(int id)
{
    crow::json::wvalue lobbyDataJson = m_player->GetLobbyDetails(id);
    try {
        int playerCount = lobbyDataJson["players"].size();
        LoadLobby(id, playerCount);
    }
    catch (const std::exception& e) {
        std::cerr << "Error converting lobby ID: " << e.what() << std::endl;
    }
}



void LobbyWindow::LoadLobby(int id, int playerCount)
{
    if (m_lobbyData.empty()) {
        m_player->SetLobbyId(id);
        m_player->JoinLobby(id);
        m_player->SetHost(true);
        m_player->SetReady();
    }
    else {
        for (auto it = m_lobbyData.begin(); it != m_lobbyData.end(); ) {
            if (it->second != id) {
                
                m_player->SetLobbyId(id);
                m_player->JoinLobby(id);
                m_player->SetHost(true);
                m_player->SetReady();
                
                ++it; // Move to the next element if no erase happens
            }
            else {
                int row = m_lobbyList->row(it->first);  // Get the row index of the item
                if (row != -1) {
                    m_lobbyList->takeItem(row);
                    delete it->first;
                }
                it = m_lobbyData.erase(it);
            }
        }
    }
    
    QString lobbyName = QString("Room %1").arg(id);
    QString playerCountString = QString("Players %1/4").arg(playerCount);

    // Formatăm șirul de caractere
    QString formattedEntry = QString("%1 %2")
        .arg(lobbyName.leftJustified(15, ' '))  // Username completat cu spații
        .arg(playerCountString.rightJustified(15, ' '));

    // Creează un nou element în listă
    QListWidgetItem* newItem = new QListWidgetItem(formattedEntry, m_lobbyList);

    m_lobbyData.insert({ newItem, id });

    // Configurăm stilul textului
    newItem->setForeground(QBrush(QColor(0, 0, 0))); // Text negru
    newItem->setFont(QFont("Courier", 40, QFont::Bold)); // Font monospaced pentru aliniere corectă

    // Adăugăm elementul în listă
    m_lobbyList->addItem(newItem);
}

void LobbyWindow::OnPlayButtonClicked() {
    if (m_lobbyList->currentItem()) {
        QString selectedLobby = m_lobbyList->currentItem()->text();
        if (m_player->GetIsHost()) {
            //QMessageBox::information(this, "Play", "Starting game with lobby: " + selectedLobby);
            // Start the game window

            int gameId = m_player->StartGame();
            m_player->SetGameId(gameId);
            if (gameId == -1) {
                return;
            } 
            m_timer->stop();
            host = true;
            GameWindow* gameWindow = new GameWindow(*m_player);
            gameWindow->show();
            //CloseConnectionWebSocket();
            //emit LobbyWindowClosed(); // Emit semnalul când se apasă Quit
            close();
        }
        else {
            QMessageBox::information(this,"Play", "You are not the host, dummy.");
        }
    }
    else {
        QMessageBox::warning(this, "Play", "Please select a lobby to play.");
    }
}

void LobbyWindow::OnCreateLobbyButtonClicked() {
    if (m_player->GetIsHost()) {
        QMessageBox::information(this, "Play", "Can't create a lobby while hosting one");
        return;
    }
    m_player->CreateLobby();
    GetLobbiesFromServer();
    update();
    //m_player->JoinLobby()
       
}


void LobbyWindow::OnQuitButtonClicked() {
    m_player->LeaveLobby();
    delete(m_player);
    emit LobbyWindowClosed(); // Emit semnalul când se apasă Quit
    close();
}

void LobbyWindow::OnItemClicked(QListWidgetItem* item)
{
    if (m_player->GetLobbyId() != m_lobbyData[item]) {
        m_player->LeaveLobby();
        int lobbyId = m_lobbyData[item];
        QMessageBox::information(this, "Play", "You have joined the lobby " + lobbyId);
        m_player->SetHost(false);
        m_player->SetLobbyId(lobbyId);
        m_player->JoinLobby(lobbyId);
        m_player->SetReady();
        update();
    }
}

