#include <crow.h>
#include "GameManager.h"
#include "LobbyManager.h"

GameManager gameManager;
LobbyManager lobbyManager;

//ANDREI TODO sterge baza asta de date temporara si foloseste baza de date reala. Poti sa stergi direct aici, ai un comentariu mai jos in care explic restul
std::unordered_map<std::string, std::pair<int, std::string>> mockDatabase = {
    {"mario", {1, "mario"}},
    {"player2", {2, "securepassword"}}
};

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/create_lobby").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("hostId")) {
            return crow::response(400, "Invalid request");
        }

        int hostId = json["hostId"].i();
        int lobbyId = lobbyManager.CreateLobby(hostId);

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

        if (lobbyManager.AddPlayerToLobby(lobbyId, playerId)) {
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

        auto* lobby = lobbyManager.GetLobby(lobbyId);
        if (!lobby || lobby->GetHostId() != hostId) {
            return crow::response(403, "Only the host can delete the lobby");
        }

        if (lobbyManager.DeleteLobby(lobbyId)) {
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

        if (lobbyManager.RemovePlayerFromLobby(lobbyId, playerId)) {
            return crow::response(200, "Player removed from lobby");
        }
        else {
            return crow::response(400, "Failed to leave lobby");
        }
        });

    // Returns the IDs of active lobbies
    CROW_ROUTE(app, "/get_active_lobbies").methods(crow::HTTPMethod::GET)([]() {
        auto activeLobbies = lobbyManager.GetActiveLobbyIds();

        crow::json::wvalue response;
        crow::json::wvalue lobbyList = crow::json::wvalue::list();

        size_t index = 0;
        for (const auto& lobbyId : activeLobbies) {
            lobbyList[index++] = lobbyId;
        }

        response["lobbies"] = std::move(lobbyList);
        return crow::response(response);
        });


    // Sets a player ready/not ready to start the game in a lobby
    CROW_ROUTE(app, "/set_ready").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId") || !json.has("isReady")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();
        bool isReady = json["isReady"].b();

        auto* lobby = lobbyManager.GetLobby(lobbyId);
        if (lobby) {
            lobby->SetReady(playerId, isReady);
            return crow::response(200, "Ready status updated");
        }
        else {
            return crow::response(404, "Lobby not found");
        }
        });

    // Returns the information needed to display everything about a lobby in the GUI
    CROW_ROUTE(app, "/get_lobby_details").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto lobbyIdStr = req.url_params.get("lobbyId");
        if (!lobbyIdStr) {
            return crow::response(400, "Missing lobbyId");
        }

        int lobbyId = std::stoi(lobbyIdStr);
        auto* lobby = lobbyManager.GetLobby(lobbyId);
        if (!lobby) {
            return crow::response(404, "Lobby not found");
        }

        crow::json::wvalue response;
        response["lobbyId"] = lobby->GetLobbyId();
        response["hostId"] = lobby->GetHostId();

        crow::json::wvalue playersJson = crow::json::wvalue::list();
        size_t index = 0;
        for (const auto& [playerId, isReady] : lobby->GetPlayers()) {
            crow::json::wvalue playerJson;
            playerJson["playerId"] = playerId;
            playerJson["isReady"] = isReady;
            playersJson[index++] = std::move(playerJson);
        }
        response["players"] = std::move(playersJson);

        return crow::response(response);
        });

    // Starts a game from a lobby if conditions are met
    CROW_ROUTE(app, "/start_game").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("lobbyId") || !json.has("playerId")) {
            return crow::response(400, "Invalid request");
        }

        int lobbyId = json["lobbyId"].i();
        int playerId = json["playerId"].i();

        auto* lobby = lobbyManager.GetLobby(lobbyId);
        if (!lobby) {
            return crow::response(404, "Lobby not found");
        }

        // Check if the player is the host, only he can start the game
        if (lobby->GetHostId() != playerId) {
            return crow::response(403, "Only the host can start the game");
        }

        // Check if the lobby meets the minimum player requirement
        if (!lobby->HasMinimumPlayers()) {
            return crow::response(400, "Not enough players to start the game");
        }

        try {
            int gameId = gameManager.CreateGameFromLobby(lobbyId);
            lobbyManager.DeleteLobby(lobbyId);
            gameManager.StartGameLoop(gameId);

            crow::json::wvalue response;
            response["gameId"] = gameId;
            return crow::response(response);
        }
        catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
        });

    // Validates the username and password and returns the playerId associated with the credentials
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid request");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();


        /*
        ANDREI TODO acum folosesc o baza de date de test care e un unordered map.Aici primesc de la client usernameul si parola introduse si trebuie verificat
        daca exista si sunt corecte in baza de date reala. Daca exista si sunt valide returneaza ID ul unic al playerului asociat cu acele date.
        Trebuie sa modifici doar ce am pus cu //TEMP .Acolo am folosit baza asta de date temporara, restul lasa asa
        */
        auto it = mockDatabase.find(username);  //TEMP 
        if (it != mockDatabase.end() && it->second.second == password) {  //TEMP intra in if ul asta doar daca ai gasit userul
            int playerId = it->second.first;  //TEMP dar tot trebuie sa pui in variabila playerId ce gasesti in baza de date pentru user

            crow::json::wvalue response;
            response["playerId"] = playerId;
            return crow::response(response);
        }

        return crow::response(401, "Invalid credentials");
        });

    // Returns the state of the game serialized. Everything the client needs to know from the server while in the game. Things like player's health, coordinates and direction, bullet coordinates and direction, map changes.
    CROW_ROUTE(app, "/game_state").methods(crow::HTTPMethod::GET)([](const crow::request& req) {

        auto gameIdStr = req.url_params.get("gameId");
        if (!gameIdStr) {
            return crow::response(400, "Missing gameId");
        }

        int gameId = std::stoi(gameIdStr);
        auto* gameState = gameManager.GetGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        auto jsonResponse = gameState->ToJson();

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

        return crow::response(gameState->ArenaToJson().dump());
        });

    CROW_ROUTE(app, "/player_move").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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

        auto* gameState = gameManager.GetGameState(gameId);
        if (!gameState) {
            return crow::response(404, "Game not found");
        }

        gameState->ProcessMove(playerId, Vector2(deltaX, deltaY), Vector2(mouseX, mouseY));

        return crow::response(200, "Movement registered");
        });

    CROW_ROUTE(app, "/shoot_weapon").methods(crow::HTTPMethod::POST)([](const crow::request& req) {

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

        return crow::response(200, "Shooting registered");
        });

    // Start server
    app.port(8080).multithreaded().run();
}
