#include "PlayWindow.h"

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

    // Conectează butonul Quit Game la funcția de închidere a ferestrei
    connect(m_quitButton, &QPushButton::clicked, this, &PlayWindow::close);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

