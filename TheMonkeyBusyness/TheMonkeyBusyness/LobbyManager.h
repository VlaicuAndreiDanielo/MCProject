#pragma once
#include "Lobby.h"
#include <unordered_map>
#include <mutex>

class LobbyManager {
public:
    LobbyManager();

    // Lobby Management
    int createLobby(int hostId);
    bool deleteLobby(int lobbyId);
    Lobby* getLobby(int lobbyId);

    // Player-Lobby Management
    bool addPlayerToLobby(int lobbyId, int playerId);
    bool removePlayerFromLobby(int lobbyId, int playerId);

    // Get list of active lobbies
    std::vector<int> getActiveLobbyIds() const;

private:
    std::unordered_map<int, Lobby> lobbies; // Map of lobbyId to Lobby objects
    int nextLobbyId;                        // For generating unique lobby IDs
    mutable std::mutex lobbyMutex;          // Protect access to lobbies
};
