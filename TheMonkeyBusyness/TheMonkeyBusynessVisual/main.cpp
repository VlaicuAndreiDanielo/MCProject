#include "Player.h"
#include "GameWindow.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::string serverUrl = "http://localhost:8080";
    std::string username = "mario";
    std::string password = "mario";

    int playerId = Player::LogIn(serverUrl, username, password);
    if (playerId == -1) return -1;

    Player player(playerId, serverUrl);

    // Create or join lobby
    int lobbyId = player.CreateLobby();
    if (lobbyId == -1) return -1;


    // Set readiness and fetch lobby details
    player.SetReady();
    auto lobbyDetails = player.GetLobbyDetails();

    // Start the game and get the gameId
    int gameId = player.StartGame();  // Start the game and get gameId
    if (gameId == -1) {
        return -1;
    }

    // Start the game window
    GameWindow gameWindow(player);
    gameWindow.show();

    return app.exec();
}
