#include "LobbyManager.h"
#include <stdexcept>

LobbyManager::LobbyManager() : nextLobbyId(1) {}

int LobbyManager::createLobby(int hostId) {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    int lobbyId = nextLobbyId++;
    lobbies.emplace(lobbyId, Lobby(lobbyId, hostId));

    return lobbyId;
}

bool LobbyManager::deleteLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    if (lobbies.find(lobbyId) == lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    lobbies.erase(lobbyId);
    return true;
}

Lobby* LobbyManager::getLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    auto it = lobbies.find(lobbyId);
    return it != lobbies.end() ? &it->second : nullptr;
}

bool LobbyManager::addPlayerToLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    auto it = lobbies.find(lobbyId);
    if (it == lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    return it->second.addPlayer(playerId);
}

bool LobbyManager::removePlayerFromLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    auto it = lobbies.find(lobbyId);
    if (it == lobbies.end()) {
        return false; // Lobby doesn't exist
    }

    it->second.removePlayer(playerId);

    // If the lobby becomes empty, delete it automatically
    if (it->second.getPlayers().empty()) {
        lobbies.erase(lobbyId);
    }

    return true;
}

std::vector<int> LobbyManager::getActiveLobbyIds() const {
    std::lock_guard<std::mutex> lock(lobbyMutex);

    std::vector<int> lobbyIds;
    for (const auto& [lobbyId, lobby] : lobbies) {
        if (lobby.getStatus() == LobbyStatus::Waiting) {
            lobbyIds.push_back(lobbyId);
        }
    }

    return lobbyIds;
}
