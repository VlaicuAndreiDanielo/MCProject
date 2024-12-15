#pragma once
#include <unordered_map>
#include <string>
#include <vector>

enum class LobbyStatus {
    Waiting,
    InGame,
    Finished
};

class Lobby {
public:
    explicit Lobby(int lobbyId, int hostId);

    // Player Management
    bool addPlayer(int playerId);
    bool removePlayer(int playerId);
    void setReady(int playerId, bool isReady);
    bool isAllReady() const;

    // Game Management
    bool startGame(int& gameId); // Returns a game ID if successful
    void resetLobby();
    bool hasMinimumPlayers() const;    // Helper to check if there are enough players

    // Accessors
    int getLobbyId() const;
    int getHostId() const;
    LobbyStatus getStatus() const;
    std::unordered_map<int, bool> getPlayers() const;

private:
    int lobbyId;                       // Unique identifier for the lobby
    int hostId;                        // Player ID of the host
    LobbyStatus status;                // Current status of the lobby
    std::unordered_map<int, bool> players; // Players and their ready status

};
