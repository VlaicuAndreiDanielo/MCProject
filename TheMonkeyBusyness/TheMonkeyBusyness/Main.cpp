#include <crow.h>
#include "GameManager.h"
#include "LobbyManager.h"

GameManager gameManager;
LobbyManager lobbyManager;

// Mock database
std::unordered_map<std::string, std::pair<int, std::string>> mockDatabase = {
    {"mario", {1, "mario"}},
    {"player2", {2, "securepassword"}}
};

int main() {
    crow::SimpleApp app;

    // Route: Create Lobby
    CROW_ROUTE(app, "/create_lobby").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("hostId")) {
            return crow::response(400, "Invalid request");
        }

        int hostId = json["hostId"].i();
        int lobbyId = lobbyManager.createLobby(hostId); // Fixed method call

        crow::json::wvalue response;
        response["lobbyId"] = lobbyId;
        return crow::response(response);
        });

    CROW_ROUTE(app, "/join_lobby").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();

        if (lobbyManager.addPlayerToLobby(lobbyId, playerId)) {
            return crow::response(200, "Player added to lobby");
        }
        else {
            return crow::response(400, "Failed to join lobby");
        }
        });

    CROW_ROUTE(app, "/delete_lobby").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("hostId")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int hostId = json["hostId"].i();

        auto* lobby = lobbyManager.getLobby(lobbyId);
        if (!lobby || lobby->getHostId() != hostId) {
            return crow::response(403, "Only the host can delete the lobby");
        }

        if (lobbyManager.deleteLobby(lobbyId)) {
            return crow::response(200, "Lobby deleted");
        }
        else {
            return crow::response(400, "Failed to delete lobby");
        }
        });



    CROW_ROUTE(app, "/leave_lobby").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();

        if (lobbyManager.removePlayerFromLobby(lobbyId, playerId)) {
            return crow::response(200, "Player removed from lobby");
        }
        else {
            return crow::response(400, "Failed to leave lobby");
        }
        });


    CROW_ROUTE(app, "/get_active_lobbies").methods(crow::HTTPMethod::GET)([]() {
        auto activeLobbies = lobbyManager.getActiveLobbyIds();

        crow::json::wvalue response;
        crow::json::wvalue lobbyList = crow::json::wvalue::list();

        size_t index = 0; // Index for list items
        for (const auto& lobbyId : activeLobbies) {
            lobbyList[index++] = lobbyId; // Add lobbyId to the list
        }

        response["lobbies"] = std::move(lobbyList); // Set the "lobbies" key in the response
        return crow::response(response);
        });



    CROW_ROUTE(app, "/set_ready").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId") || !json.has("isReady")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();
        bool isReady = json["isReady"].b();

        auto* lobby = lobbyManager.getLobby(lobbyId);
        if (lobby) {
            lobby->setReady(playerId, isReady);
            return crow::response(200, "Ready status updated");
        }
        else {
            return crow::response(404, "Lobby not found");
        }
        });

    CROW_ROUTE(app, "/get_lobby_details").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto lobbyIdStr = req.url_params.get("lobbyId");
        if (!lobbyIdStr) {
            return crow::response(400, "Missing lobbyId");
        }

        int lobbyId = std::stoi(lobbyIdStr);
        auto* lobby = lobbyManager.getLobby(lobbyId);
        if (!lobby) {
            return crow::response(404, "Lobby not found");
        }

        crow::json::wvalue response;
        response["lobbyId"] = lobby->getLobbyId();
        response["hostId"] = lobby->getHostId();
        response["status"] = static_cast<int>(lobby->getStatus());

        crow::json::wvalue playersJson = crow::json::wvalue::list();
        size_t index = 0;
        for (const auto& [playerId, isReady] : lobby->getPlayers()) {
            crow::json::wvalue playerJson;
            playerJson["playerId"] = playerId;
            playerJson["isReady"] = isReady;
            playersJson[index++] = std::move(playerJson);
        }
        response["players"] = std::move(playersJson);

        return crow::response(response);
        });



    CROW_ROUTE(app, "/start_game").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();

        auto* lobby = lobbyManager.getLobby(lobbyId);
        if (!lobby) {
            return crow::response(404, "Lobby not found");
        }

        // Check if the player is the host
        if (lobby->getHostId() != playerId) {
            return crow::response(403, "Only the host can start the game");
        }

        // Check if the lobby meets the minimum player requirement
        if (!lobby->hasMinimumPlayers()) {
            return crow::response(400, "Not enough players to start the game");
        }

        try {
            int gameId = gameManager.createGameFromLobby(lobbyId); // Create the game from the lobby
            lobbyManager.deleteLobby(lobbyId); // Delete the lobby
            gameManager.startGameLoop(gameId); // Start the game loop

            crow::json::wvalue response;
            response["gameId"] = gameId;
            return crow::response(response);
        }
        catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
        });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid request");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        auto it = mockDatabase.find(username);
        if (it != mockDatabase.end() && it->second.second == password) {
            int playerId = it->second.first;

            crow::json::wvalue response;
            response["playerId"] = playerId;

            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = response.dump();
            return res;
        }

        return crow::response(401, "Invalid credentials");
        });

    CROW_ROUTE(app, "/game_state").methods(crow::HTTPMethod::GET)([](const crow::request& req) {

        auto start = std::chrono::high_resolution_clock::now();

        auto gameIdStr = req.url_params.get("gameId");
        if (!gameIdStr) {
            return crow::response(400, "Missing gameId");
        }

        int gameId = std::stoi(gameIdStr);
        auto* gameState = gameManager.getGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        auto jsonStart = std::chrono::high_resolution_clock::now();
        auto jsonResponse = gameState->toJson();
        auto jsonEnd = std::chrono::high_resolution_clock::now();

        auto end = std::chrono::high_resolution_clock::now();
        std::ofstream logFile("server_log.txt", std::ios::app);
        logFile << "Processed /game_state in "
            << std::chrono::duration<double>(end - start).count()
            << " seconds." << std::endl;
        logFile << "JSON serialization took "
            << std::chrono::duration<double>(jsonEnd - jsonStart).count()
            << " seconds." << std::endl;

        return crow::response(jsonResponse.dump());
        });

    // Route to send the entire arena (used at game start or round start)
    CROW_ROUTE(app, "/game_arena").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto gameIdStr = req.url_params.get("gameId");
        if (!gameIdStr) {
            return crow::response(400, "Missing gameId");
        }

        int gameId = std::stoi(gameIdStr);
        auto* gameState = gameManager.getGameState(gameId);

        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        return crow::response(gameState->arenaToJson().dump());
        });

    CROW_ROUTE(app, "/player_move").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto start = std::chrono::high_resolution_clock::now(); // For logging

        auto json = crow::json::load(req.body);

        if (!json || !json.has("playerId") || !json.has("gameId") ||
            !json.has("deltaX") || !json.has("deltaY") ||
            !json.has("mouseX") || !json.has("mouseY")) {
            return crow::response(400, "Invalid request");
        }

        int playerId = json["playerId"].i();
        int gameId = json["gameId"].i();
        float deltaX = json["deltaX"].d();
        float deltaY = json["deltaY"].d();
        float mouseX = json["mouseX"].d();
        float mouseY = json["mouseY"].d();

        auto* gameState = gameManager.getGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        gameState->ProcessMove(playerId, Vector2(deltaX, deltaY), Vector2(mouseX, mouseY));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::ofstream logFile("server_log.txt", std::ios::app);
        logFile << "Processed /player_move in " << duration.count() << " seconds." << std::endl;

        return crow::response(200, "Movement registered");
        });

    CROW_ROUTE(app, "/shoot_weapon").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto start = std::chrono::high_resolution_clock::now(); // For logging

        auto json = crow::json::load(req.body);

        if (!json || !json.has("playerId") || !json.has("gameId") || !json.has("mouseX") || !json.has("mouseY")) {
            return crow::response(400, "Invalid request");
        }

        int playerId = json["playerId"].i();
        int gameId = json["gameId"].i();
        float mouseX = json["mouseX"].d();
        float mouseY = json["mouseY"].d();

        auto* gameState = gameManager.getGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        gameState->ProcessShoot(playerId, Vector2(mouseX, mouseY));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::ofstream logFile("server_log.txt", std::ios::app);
        logFile << "Processed /shoot_weapon in " << duration.count() << " seconds." << std::endl;

        return crow::response(200, "Shooting registered");
        });



    // Start server
    app.port(8080).multithreaded().run();
}
