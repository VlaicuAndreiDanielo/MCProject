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
	gameWindow.showFullScreen();


	return app.exec();
}