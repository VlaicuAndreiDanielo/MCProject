#include "LobbyManager.h"
#include <stdexcept>

LobbyManager::LobbyManager() : m_nextLobbyId(1) {}

int LobbyManager::CreateLobby(int hostId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    int lobbyId = m_nextLobbyId++;
    m_lobbies.emplace(lobbyId, Lobby(lobbyId, hostId));

    return lobbyId;
}

bool LobbyManager::deleteLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    if (m_lobbies.find(lobbyId) == m_lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    m_lobbies.erase(lobbyId);
    return true;
}

Lobby* LobbyManager::GetLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    return it != m_lobbies.end() ? &it->second : nullptr;
}

bool LobbyManager::AddPlayerToLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    return it->second.AddPlayer(playerId);
}

bool LobbyManager::RemovePlayerFromLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    it->second.RemovePlayer(playerId);

    // If the lobby becomes empty, delete it automatically
    if (it->second.GetPlayers().empty()) {
        m_lobbies.erase(lobbyId);
    }

    return true;
}

std::vector<int> LobbyManager::GetActiveLobbyIds() const {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    std::vector<int> lobbyIds;
    for (const auto& [lobbyId, lobby] : m_lobbies) {
        if (lobby.GetStatus() == LobbyStatus::Waiting) {
            lobbyIds.push_back(lobbyId);
        }
    }

    return lobbyIds;
}
