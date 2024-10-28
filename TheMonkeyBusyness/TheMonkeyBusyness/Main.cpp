#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "GameWindow.h"
#include "InputHandler.h"
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	GameWindow gameWindow;
	InputHandler inputHandler;
	gameWindow.show();
	gameWindow.installEventFilter(&inputHandler);
	return app.exec();
}