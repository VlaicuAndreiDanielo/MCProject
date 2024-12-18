#pragma once
#include "Lobby.h"
#include "ConstantValues.h"
#include <unordered_map>
#include <mutex>

class LobbyManager {
public:
    LobbyManager();

    // Lobby Management
    int CreateLobby(int hostId);
    bool DeleteLobby(int lobbyId);
    Lobby* GetLobby(int lobbyId);

    // Player-Lobby Management
    bool AddPlayerToLobby(int lobbyId, int playerId);
    bool RemovePlayerFromLobby(int lobbyId, int playerId);

    // Get list of active lobbies
    std::vector<int> GetActiveLobbyIds() const;

private:
    std::unordered_map<int, Lobby> m_lobbies;
    int m_nextLobbyId;  // For generating unique lobby IDs
    mutable std::mutex m_lobbyMutex;
};
