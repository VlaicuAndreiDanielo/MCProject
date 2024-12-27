#include <QApplication>
#include "firstmainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    FirstMainWindow window;
    window.show();
    return app.exec();
}
