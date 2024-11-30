#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "GameWindow.h"
int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    GameWindow gameWindow;
    gameWindow.showFullScreen();

    return app.exec();
}
