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
		player.UpdateRotation(player.ReturnInputHandler()->mousePosition);
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


	arena.draw(painter);

    player.draw(painter);

   
    // Restore painter to its previous state
    painter.restore();
}
