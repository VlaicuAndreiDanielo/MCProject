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

    // Create or join lobby
    //int lobbyId = player.CreateLobby();
    //if (lobbyId == -1) return -1;

    //// Set readiness and fetch lobby details
    //player.SetReady();
    //auto lobbyDetails = player.GetLobbyDetails();

    // Start the game and get the gameId
    //int gameId = player.StartGame();  // Start the game and get gameId
    //if (gameId == -1) {
    //    return -1;
    //}

    FirstMainWindow firstWindow;
    firstWindow.show();
    

    return app.exec();
}
