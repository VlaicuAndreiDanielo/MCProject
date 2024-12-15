#include <crow.h>
#include "GameManager.h"

GameManager gameManager;

//TODO implement real database and remove this. Mock database: Maps usernames to player IDs and passwords
std::unordered_map<std::string, std::pair<int, std::string>> mockDatabase = {
    {"player1", {1, "password123"}},
    {"player2", {2, "securepassword"}},
};

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/create_game").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("playerId")) {
            return crow::response(400, "Invalid request");
        }

        int playerId = json["playerId"].i();
        int gameId = gameManager.CreateGame();

        gameManager.AddPlayerToGame(gameId, playerId);

        crow::json::wvalue response;
        response["gameId"] = gameId;
        return crow::response(response);
        });

    CROW_ROUTE(app, "/start_game_loop").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        try {
            auto json = crow::json::load(req.body);
            if (!json || !json.has("gameId")) {
                return crow::response(400, "Invalid request");
            }

            int gameId = json["gameId"].i();
            gameManager.StartGameLoop(gameId);

            return crow::response(200, "Game loop started");
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in /start_game_loop: " << e.what() << std::endl;
            return crow::response(500, std::string("Internal Server Error: ") + e.what());
        }
        catch (...) {
            std::cerr << "Unknown error in /start_game_loop." << std::endl;
            return crow::response(500, "Unknown Internal Server Error");
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

    CROW_ROUTE(app, "/player_move").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto start = std::chrono::high_resolution_clock::now(); // For logging

        auto json = crow::json::load(req.body);

        // Validate request
        if (!json || !json.has("playerId") || !json.has("gameId") ||
            !json.has("deltaX") || !json.has("deltaY") ||
            !json.has("mouseX") || !json.has("mouseY")) {
            return crow::response(400, "Invalid request");
        }

        // Extract data from the request
        int playerId = json["playerId"].i();
        int gameId = json["gameId"].i();
        float deltaX = json["deltaX"].d();
        float deltaY = json["deltaY"].d();
        float mouseX = json["mouseX"].d();
        float mouseY = json["mouseY"].d();

        // Get the game state
        auto* gameState = gameManager.GetGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        // Process movement and direction updates
        gameState->ProcessMove(playerId, Vector2(deltaX, deltaY), Vector2(mouseX, mouseY));

        // Log the processing duration
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::ofstream logFile("server_log.txt", std::ios::app);
        logFile << "Processed /player_move in " << duration.count() << " seconds." << std::endl;

        return crow::response(200, "Movement registered");
        });



    // Player shooting
    CROW_ROUTE(app, "/shoot_weapon").methods(crow::HTTPMethod::POST)([](const crow::request& req) {

        auto start = std::chrono::high_resolution_clock::now(); //for logging

        auto json = crow::json::load(req.body);

        if (!json || !json.has("playerId") || !json.has("gameId") || !json.has("mouseX") || !json.has("mouseY")) {
            return crow::response(400, "Invalid request");
        }

        int playerId = json["playerId"].i();
        int gameId = json["gameId"].i();
        float mouseX = json["mouseX"].d();
        float mouseY = json["mouseY"].d();

        auto* gameState = gameManager.GetGameState(gameId);
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


    CROW_ROUTE(app, "/game_state").methods(crow::HTTPMethod::GET)([](const crow::request& req) {

        auto start = std::chrono::high_resolution_clock::now();

        auto gameIdStr = req.url_params.get("gameId");
        if (!gameIdStr) {
            return crow::response(400, "Missing gameId");
        }

        int gameId = std::stoi(gameIdStr);
        auto* gameState = gameManager.GetGameState(gameId);
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
        auto* gameState = gameManager.GetGameState(gameId);

        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        return crow::response(gameState->arenaToJson().dump());
        });

    // Start server
    app.port(8080).multithreaded().run();
}
