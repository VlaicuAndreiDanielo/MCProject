#include "Lobby.h"
#include <stdexcept>
#include "ConstantValues.h"

Lobby::Lobby(int lobbyId, int hostId)
    : m_lobbyId(lobbyId), m_hostId(hostId) {
    m_players[hostId] = false; // Host is initially not ready
}

bool Lobby::AddPlayer(int playerId) {
    if (m_players.size() >= HasMaximumPlayers()) {
        return false; // Lobby is full
    }
    m_players[playerId] = false;
    return true;
}

bool Lobby::RemovePlayer(int playerId) {
    if (m_players.find(playerId) == m_players.end()) {
        return false; // Player not in lobby
    }

    m_players.erase(playerId);

    // If host leaves, assign a new host
    if (playerId == m_hostId && !m_players.empty()) {
        m_hostId = m_players.begin()->first;
        //TODO tell the clients who is the new host,the new host should know he is the new host. One idea is to periodically check the lobby status and send trough that the hostId and add a check in the client side for the host
    }

    return true;
}

void Lobby::SetReady(int playerId, bool isReady) {
    if (m_players.find(playerId) != m_players.end()) {
        m_players[playerId] = isReady;
    }
}

bool Lobby::IsAllReady() const {
    for (const auto& [playerId, isReady] : m_players) {
        if (!isReady) {
            return false;
        }
    }
    return true;
}

int Lobby::GetLobbyId() const {
    return m_lobbyId;
}

int Lobby::GetHostId() const {
    return m_hostId;
}

std::unordered_map<int, bool> Lobby::GetPlayers() const {
    return m_players;
}

bool Lobby::HasMinimumPlayers() const {
    return m_players.size() >= GameConfig::kMinLobbyPlayers;
}

bool Lobby::HasMaximumPlayers() const {
    return m_players.size() >= GameConfig::kMaxLobbyPlayers;
}
