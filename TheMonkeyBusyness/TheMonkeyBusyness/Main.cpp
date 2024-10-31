#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "GameWindow.h"
#include "Player.h"
#include "InputHandler.h"
#include <QtCore/QTimer>
#include <QtCore/qobject.h>
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	GameWindow gameWindow;
	Player player;
	gameWindow.show();
	gameWindow.installEventFilter(player.ReturnInputHandler());

	//Temporary simulation of an Update loop
	QTimer* timer = new QTimer(&gameWindow);
	QObject::connect(timer, &QTimer::timeout, [&player]() {
		player.UpdatePosition(player.ReturnInputHandler()->direction);
		});
	timer->start(16);
	return app.exec();
}