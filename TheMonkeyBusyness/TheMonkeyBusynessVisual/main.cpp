#include "Player.h"
#include "GameWindow.h"
#include "FirstMainWindow.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);



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
