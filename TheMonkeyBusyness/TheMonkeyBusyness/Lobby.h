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
    bool AddPlayer(int playerId);
    bool RemovePlayer(int playerId);
    void SetReady(int playerId, bool isReady);
    bool IsAllReady() const;

    // Game Management
    bool StartGame(int& gameId); // Returns a game ID if successful
    void ResetLobby();
    bool HasMinimumPlayers() const;    // Helper to check if there are enough players

    // Accessors
    int GetLobbyId() const;
    int GetHostId() const;
    LobbyStatus GetStatus() const;
    std::unordered_map<int, bool> GetPlayers() const;

private:
    int m_lobbyId;                       // Unique identifier for the lobby
    int m_hostId;                        // Player ID of the host
    LobbyStatus m_status;                // Current status of the lobby
    std::unordered_map<int, bool> m_players; // Players and their ready status

};
