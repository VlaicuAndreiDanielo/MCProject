#include "Player.h"
#include "GameWindow.h"
#include <QtWidgets/qapplication.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::string serverUrl = "http://localhost:8080";
    std::string username = "player1";
    std::string password = "password123";

    int playerId = Player::logIn(serverUrl, username, password);
    if (playerId == -1) return -1;

    int gameId = Player::createGame(serverUrl, playerId);
    if (gameId == -1) return -1;

    Player player(playerId, serverUrl);
    player.setGameId(gameId);
    player.startGame(gameId);

    GameWindow gameWindow(player);
    gameWindow.show();

    return app.exec();
}
