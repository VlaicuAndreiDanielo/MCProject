#include "EndGameWindow.h"
EndGameWindow::EndGameWindow(Player* player, QWidget* parent) : m_player(player), QWidget(parent)
{
	resize(800, 600);
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QLabel* cornerText = new QLabel("Corner Info", this);
    QHBoxLayout* cornerLayout = new QHBoxLayout();
    cornerLayout->addWidget(cornerText);
    cornerLayout->addStretch(); // Pushes the text to the left corner
    mainLayout->addLayout(cornerLayout);

    // Title
    QLabel* title = new QLabel("Title Goes Here", this);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Point Bar and Button2
    QHBoxLayout* barLayout = new QHBoxLayout();
    m_pointBar = new QProgressBar(this);
    m_pointBar->setRange(0, 5); // Max is 5
    QPushButton* button2 = new QPushButton("+", this);

    barLayout->addWidget(m_pointBar);
    barLayout->addWidget(button2);
    mainLayout->addLayout(barLayout);

    // Countdown Timer
    m_countdownLabel = new QLabel("Countdown: 15", this);
    m_countdownLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_countdownLabel);

    QPushButton* m_button = new QPushButton("Click Me!", this);
    mainLayout->addWidget(m_button);

    // Connect button clicked signal
    connect(m_button, &QPushButton::clicked, this, &EndGameWindow::OnButtonClicked);

    //// Connect button2 (point upgrade button)
    connect(button2, &QPushButton::clicked, this, &EndGameWindow::OnPointUpgradeClicked);

    //// Set up the timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EndGameWindow::UpdateCountdown);
    m_timer->start(1000);
}

void EndGameWindow::OnButtonClicked() {

}

void EndGameWindow::OnPointUpgradeClicked()
{
}

void EndGameWindow::UpdateCountdown()
{
}
