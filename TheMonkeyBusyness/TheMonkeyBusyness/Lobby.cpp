#include "Lobby.h"
#include <stdexcept>
#include "ConstantValues.h"

Lobby::Lobby(int lobbyId, int hostId)
    : m_lobbyId(lobbyId), m_hostId(hostId), m_status(LobbyStatus::Waiting) {
    m_players[hostId] = false; // Host is initially not ready
}

bool Lobby::AddPlayer(int playerId) {
    if (m_status != LobbyStatus::Waiting || m_players.size() >= GameConfig::kMaxLobbyPlayers) {
        return false; // Lobby is full or already in-game
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

bool Lobby::StartGame(int& gameId) {
    if (m_status != LobbyStatus::Waiting || !HasMinimumPlayers() || !IsAllReady()) {
        return false; // Can't start game
    }

    m_status = LobbyStatus::InGame;
    gameId = m_lobbyId; // Example: Use lobbyId as gameId for simplicity
    return true;
}

void Lobby::ResetLobby() {
    m_status = LobbyStatus::Waiting;
    for (auto& [playerId, isReady] : m_players) {
        isReady = false;
    }
}

int Lobby::GetLobbyId() const {
    return m_lobbyId;
}

int Lobby::GetHostId() const {
    return m_hostId;
}

LobbyStatus Lobby::GetStatus() const {
    return m_status;
}

std::unordered_map<int, bool> Lobby::GetPlayers() const {
    return m_players;
}

bool Lobby::HasMinimumPlayers() const {
    return m_players.size() >= GameConfig::kMinLobbyPlayers;
}
