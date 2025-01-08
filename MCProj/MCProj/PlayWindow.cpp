#include "PlayWindow.h"
#include "LobbyWindow.h"
#include <QMessageBox>

PlayWindow::PlayWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Game Window");
    showFullScreen(); // Setează fereastra fullscreen

    // **Setează imaginea ca fundal**
    QString imagePath = QCoreApplication::applicationDirPath() + "/Images/background_playwindow.jpg";
    QPixmap background(imagePath);
    if (!background.isNull()) {
        background = background.scaled(size(), Qt::KeepAspectRatioByExpanding);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background));
        setPalette(palette);
    }

    // Creează widgetul central și layout-ul
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Creează titlul
    QLabel* titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 56, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: rgba(255, 223, 0, 255);");
    titleLabel->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect* shadowEffectTitle = new QGraphicsDropShadowEffect(this);
    shadowEffectTitle->setOffset(4, 4);
    shadowEffectTitle->setBlurRadius(15);
    shadowEffectTitle->setColor(QColor(0, 0, 0, 150));
    titleLabel->setGraphicsEffect(shadowEffectTitle);

    layout->addWidget(titleLabel); // Adaugă titlul în layout

    // Creează butonul Play
    m_playButton = new QPushButton("Play", this);
    m_playButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 215, 0, 200);
            color: black;
            font-size: 32px;
            font-weight: bold;
            border-radius: 15px;
            padding: 20px;
        }
        QPushButton:hover {
            background-color: rgba(255, 200, 0, 230);
        }
    )");
    m_playButton->setFixedSize(400, 100);

    // Adaugă un efect de umbră pentru buton
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(4, 4);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    m_playButton->setGraphicsEffect(shadowEffect);

    // Adaugă butonul la layout și centrează-l
    layout->addWidget(m_playButton, 0, Qt::AlignCenter);
    centralWidget->setLayout(layout);

    // Creează butonul Controls
    m_controlsButton = new QPushButton("Controls", this);
    m_controlsButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(100, 200, 255, 200);
            color: black;
            font-size: 24px;
            font-weight: bold;
            border-radius: 15px;
            padding: 15px;
        }
        QPushButton:hover {
            background-color: rgba(80, 180, 255, 230);
        }
    )");
    m_controlsButton->setFixedSize(300, 80);
    layout->addWidget(m_controlsButton, 0, Qt::AlignCenter); // Adaugă butonul Controls

    // Butonul Quit Game
    m_quitButton = new QPushButton("Quit Game", this);
    m_quitButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255, 100, 100, 200);
            color: white;
            font-size: 24px;
            font-weight: bold;
            border-radius: 15px;
            padding: 15px;
        }
        QPushButton:hover {
            background-color: rgba(255, 70, 70, 230);
        }
    )");
    m_quitButton->setFixedSize(300, 80);
    layout->addWidget(m_quitButton, 0, Qt::AlignCenter);

    connect(m_playButton, &QPushButton::clicked, this, &PlayWindow::openLobbyWindow);
    // Conectează butonul Quit Game la funcția de închidere a ferestrei
    connect(m_quitButton, &QPushButton::clicked, this, &PlayWindow::close);
    // Conectare buton pt controale
    connect(m_controlsButton, &QPushButton::clicked, this, &PlayWindow::showControls);


    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void PlayWindow::openLobbyWindow() {
    if (!m_lobbyWindow) { // Creează fereastra doar dacă nu există deja
        m_lobbyWindow = new LobbyWindow();
        connect(m_lobbyWindow, &LobbyWindow::lobbyWindowClosed, this, &PlayWindow::handleLobbyWindowClosed);

        m_lobbyWindow->showFullScreen(); // Afișează fereastra în modul fullscreen

        // Ascundem PlayWindow când LobbyWindow este deschis
        hide();
    }
}
void PlayWindow::handleLobbyWindowClosed() {
    m_lobbyWindow = nullptr; // Resetăm pointerul
    show(); // Afișăm PlayWindow din nou, dacă a fost ascuns
}

void PlayWindow::showControls() {
    // Afișează controalele jocului într-un QMessageBox
    QString controlsText = R"(
        <b>Controls:</b><br>
        Move Forward: W<br>
        Move Backward: S<br>
        Move Left: A<br>
        Move Right: D<br>
        Shoot: Space<br>
    )";

    QMessageBox::information(this, "Game Controls", controlsText);
}