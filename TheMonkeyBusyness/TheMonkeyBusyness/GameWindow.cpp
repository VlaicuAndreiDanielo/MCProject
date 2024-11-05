#include "GameWindow.h"
#include <QtCore/QTimer>
#include <QtCore/qobject.h>
GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
	setFixedSize(800, 800);
	setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(player.ReturnInputHandler());
	timer = new QTimer(this);
	QObject::connect(timer, &QTimer::timeout, [this]() {
		player.UpdatePosition(player.ReturnInputHandler()->direction);
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

    // Move the origin to the center of the widget
    int centerX = width() / 2;
    int centerY = height() / 2;
    painter.translate(centerX, centerY);

    // Rotate around the center of the widget
   // painter.rotate(angle);

    // Draw a square centered at the painter's origin (now at widget center)
    int squareSize = 100;
    painter.drawRect(-squareSize / 2, -squareSize / 2, squareSize, squareSize);

    // Restore painter to its previous state
    painter.restore();
}
