#include "LobbyWindow.h"
#include <QMessageBox>

LobbyWindow::LobbyWindow(QWidget* parent) : QWidget(parent) {
    setupUI();
    showFullScreen(); // Afișează fereastra în mod full screen
}

void LobbyWindow::setupUI() {
    // Setează titlul ferestrei
    setWindowTitle("Monkeys Madness");
    setFixedSize(800, 600);

    // Creează titlul
    m_titleLabel = new QLabel("Monkeys Madness", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Creează butoanele
    m_playButton = new QPushButton("Play", this);
    m_playButton->setEnabled(false); // Inițial dezactivat
    m_createLobbyButton = new QPushButton("Create Lobby", this);
    m_quitButton = new QPushButton("Quit", this);

    // Creează lista pentru lobby-uri
    m_lobbyList = new QListWidget(this);
    m_lobbyList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Conectează acțiunea de selecție la activarea butonului "Play"
    connect(m_lobbyList, &QListWidget::itemSelectionChanged, [=]() {
        m_playButton->setEnabled(m_lobbyList->selectedItems().size() > 0);
        });

    // Conectează butoanele la sloturi
    connect(m_playButton, &QPushButton::clicked, this, &LobbyWindow::onPlayButtonClicked);
    connect(m_createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onCreateLobbyButtonClicked);
    connect(m_quitButton, &QPushButton::clicked, this, &LobbyWindow::onQuitButtonClicked);

    // Layout pentru butoane
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(m_playButton);
    buttonLayout->addWidget(m_createLobbyButton);
    buttonLayout->addWidget(m_quitButton);
    buttonLayout->addStretch();

    // Layout pentru întreaga fereastră
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* leftLayout = new QVBoxLayout();

    leftLayout->addWidget(m_titleLabel);
    leftLayout->addLayout(buttonLayout);

    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(m_lobbyList);

    setLayout(mainLayout);
}

void LobbyWindow::onPlayButtonClicked() {
    if (m_lobbyList->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a lobby before playing!");
        return;
    }
    QString selectedLobby = m_lobbyList->selectedItems().first()->text();
    QMessageBox::information(this, "Playing", "You are now joining the lobby: " + selectedLobby);
    // Aici poți deschide logica jocului
}

void LobbyWindow::onCreateLobbyButtonClicked() {
    static int lobbyCounter = 1;
    QString newLobbyName = "Lobby " + QString::number(lobbyCounter++);
    m_lobbyList->addItem(newLobbyName);
    QMessageBox::information(this, "Lobby Created", "New lobby created: " + newLobbyName);
}

void LobbyWindow::onQuitButtonClicked() {
    close(); // Închide fereastra
}
