#include "MainThread.h"

MainThread::MainThread(int argc, char* argv[])
{
	QApplication app(argc, argv);
	GameWindow game;
	game.show();
	return app.exec();
}
