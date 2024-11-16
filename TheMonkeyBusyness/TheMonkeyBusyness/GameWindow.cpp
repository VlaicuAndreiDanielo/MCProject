#include "GameWindow.h"
#include <QtCore/QTimer>
#include <QtCore/qobject.h>
GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
	arena.generate_map(300,1);
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(player.ReturnInputHandler());
	timer = new QTimer(this);
	QObject::connect(timer, &QTimer::timeout, [this]() {
		player.UpdatePosition(player.ReturnInputHandler()->direction);
		player.UpdateRotation(player.ReturnInputHandler()->mousePosition, width(), height());
		update();
		});
	timer->start(16);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Save the painter's current state
    painter.save();
    int screenWidth = width();
    int screenHeight = height();

    // Player's position 
    int playerX = player.GetPosition().x;
    int playerY = player.GetPosition().y;

    // Define the size of the visible area 
    int viewWidth = screenWidth;
    int viewHeight = screenHeight;

    // Translate the painter's coordinate system to shift the map, centering the player
    painter.translate(screenWidth / 2 - playerX, screenHeight / 2 - playerY);
	arena.draw(painter);

    player.draw(painter);

   
    // Restore painter to its previous state
    painter.restore();
}
