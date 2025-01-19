#include "Player.h"
#include "GameWindow.h"
#include "FirstMainWindow.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <fstream>
#include "Constants.h"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::fstream file("config.txt"); // Open the file
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    std::string line;
    std::string content;

    // Read the file line by line
    while (std::getline(file, line)) {
        content += line; // Add the line and a newline character
    }

    file.close();
    gameserverUrl = content;

    FirstMainWindow firstWindow;
    firstWindow.show();
    

    return app.exec();
}
