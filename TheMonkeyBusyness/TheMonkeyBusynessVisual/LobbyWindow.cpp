/*Versiunea cu QListWidget pt lobbyuri*/
#include "LobbyWindow.h"
#include "SessionManager.h"
#include <QGraphicsDropShadowEffect>
#include <QCoreApplication>
#include <QPixmap>
#include <QPalette>
#include <QMessageBox>


LobbyWindow::LobbyWindow(QWidget* parent) : QWidget(parent) {
    SetupUI();
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
    m_playButton = new QPushButton("Play", this);
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
}

void LobbyWindow::OnPlayButtonClicked() {
    if (m_lobbyList->currentItem()) {
        QString selectedLobby = m_lobbyList->currentItem()->text();
        QMessageBox::information(this, "Play", "Starting game with lobby: " + selectedLobby);
    }
    else {
        QMessageBox::warning(this, "Play", "Please select a lobby to play.");
    }
}

void LobbyWindow::OnCreateLobbyButtonClicked() {
    // Deschide un dialog pentru a cere utilizatorului să introducă numele lobby-ului
    bool ok;
    QString lobbyName = QInputDialog::getText(this, "Create Lobby",
        "Enter lobby name:", QLineEdit::Normal,
        "", &ok);

    // Dacă utilizatorul a dat click pe OK și a introdus un nume valid
    if (ok && !lobbyName.trimmed().isEmpty()) {
        // Obținem numele utilizatorului conectat (de exemplu, "Player1")
        QString username = SessionManager::GetCurrentUsername();
        qDebug() << "Current logged-in user: " << username;

        // Obținem data și ora curentă
        QString currentDateTime = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");

        // Formatăm șirul de caractere
        QString formattedEntry = QString("Name: %1 Creator: %2 Date: %3")
            .arg(lobbyName.leftJustified(35, ' '))  // Numele lobby-ului completat cu spații
            .arg(username.leftJustified(35, ' '))  // Username completat cu spații
            .arg(currentDateTime.leftJustified(25, ' ')); // Data și ora completată cu spații

        // Creează un nou element în listă
        QListWidgetItem* newItem = new QListWidgetItem(formattedEntry, m_lobbyList);

        // Configurăm stilul textului
        newItem->setForeground(QBrush(QColor(0, 0, 0))); // Text negru
        newItem->setFont(QFont("Courier", 12, QFont::Bold)); // Font monospaced pentru aliniere corectă

        // Adăugăm elementul în listă
        m_lobbyList->addItem(newItem);
        QMessageBox::information(this, "Create Lobby", "New lobby created: " + lobbyName);
    }
    else {
        QMessageBox::warning(this, "Create Lobby", "Lobby name cannot be empty.");
    }
}


void LobbyWindow::OnQuitButtonClicked() {
    emit LobbyWindowClosed(); // Emit semnalul când se apasă Quit
    close();
}

