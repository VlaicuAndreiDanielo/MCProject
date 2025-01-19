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

    // Countdown Timer
    m_countdownLabel = new QLabel("Countdown: 15", this);
    m_countdownLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_countdownLabel);

    QPushButton* m_button = new QPushButton("Click Me!", this);
    mainLayout->addWidget(m_button);

    // Connect button clicked signal
    connect(m_button, &QPushButton::clicked, this, &EndGameWindow::OnButtonClicked);

    // Set up the timer
   /* m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EndGameWindow::UpdateCountdown);
    m_timer->start(1000);*/
}

void EndGameWindow::OnButtonClicked() {

}


void EndGameWindow::UpdateCountdown()
{
}

void EndGameWindow::EndGameWindowClosed()
{
}
