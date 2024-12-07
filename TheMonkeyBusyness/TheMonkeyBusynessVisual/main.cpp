#include <QApplication>
#include <cpr/cpr.h>
#include <crow/json.h>
#include "GameWindow.h"
#include <iostream>
#include <fstream>

// Redirect logs to a file
std::ofstream logFile("game_log.txt");

int loginPlayer(const std::string& serverUrl, const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/login" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" });

    std::cout << "Login response (raw): " << response.text << std::endl;

    auto jsonResponse = crow::json::load(response.text);
    if (!jsonResponse) {
        std::cerr << "Error: Failed to parse JSON response. Raw response: " << response.text << std::endl;
        abort();
    }

    if (!jsonResponse.has("playerId")) {
        std::cerr << "Error: playerId not found in JSON response. Response: " << response.text << std::endl;
        abort();
    }

    int playerId = jsonResponse["playerId"].i();
    std::cout << "Login successful! Player ID: " << playerId << std::endl;
    return playerId;
}


// Function to create a new game and return the game ID
int createGame(const std::string& serverUrl, int playerId) {
    cpr::Response response = cpr::Post(
        cpr::Url{ serverUrl + "/create_game" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"playerId":)" + std::to_string(playerId) + R"(})" });

    std::cout << "Create game response: " << response.text << std::endl;

    auto jsonResponse = crow::json::load(response.text);

    if (!jsonResponse) {
        std::cerr << "Failed to parse JSON: " << response.text << std::endl;
        abort();
    }

    if (jsonResponse && jsonResponse.has("gameId")) {
        int gameId = jsonResponse["gameId"].i();
        std::cout << "Game created successfully! Game ID: " << gameId << std::endl;
        return gameId;
    }
    else {
        std::cerr << "Error: Missing gameId in /create_game response." << std::endl;
        abort(); // Fail-fast for debugging
    }
}

void startGameLoop(const std::string& serverUrl, int gameId) {
    auto response = cpr::Post(
        cpr::Url{ serverUrl + "/start_game_loop" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ R"({"gameId":)" + std::to_string(gameId) + R"(})" }
    );

    if (response.status_code == 200) {
        std::cout << "Game loop started successfully for gameId=" << gameId << std::endl;
    }
    else {
        std::cerr << "Failed to start game loop: " << response.text << std::endl;
    }
}




int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Redirect std::cout to the log file
    std::cout.rdbuf(logFile.rdbuf());

    std::string serverUrl = "http://localhost:8080";
    std::string username = "player1";
    std::string password = "password123";

    int playerId = loginPlayer(serverUrl, username, password);

    if (playerId == -1) {
        return -1; // Exit if login fails
    }

    int gameId = createGame(serverUrl, playerId);
    startGameLoop(serverUrl, gameId);

    logFile << "Initializing GameWindow..." << std::endl;

    GameWindow gameWindow(playerId, gameId, serverUrl);
    //gameWindow.setFixedSize(800, 600); // Ensure window size is set
    gameWindow.show();

    logFile << "Event loop starting..." << std::endl;
    return app.exec();
}
