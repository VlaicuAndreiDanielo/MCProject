#include "LobbyManager.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

LobbyManager::LobbyManager() : m_nextLobbyId(GameConfig::kfirstLobbyId) {}

int LobbyManager::CreateLobby(int hostId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    int lobbyId = m_nextLobbyId++;

    auto lobby = std::make_shared<Lobby>(lobbyId, hostId);
    m_lobbies.emplace(lobbyId, lobby);

    return lobbyId;
}

bool LobbyManager::DeleteLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false;
    }

    m_lobbies.erase(it);
    return true;
}

std::shared_ptr<Lobby> LobbyManager::GetLobby(int lobbyId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it != m_lobbies.end()) {
        //std::cout << "Lobby found: ID = " << lobbyId << std::endl;
        return it->second;
    }

    //std::cerr << "Lobby not found: ID = " << lobbyId << std::endl;
    return nullptr;
}

bool LobbyManager::AddPlayerToLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false;
    }

    return it->second->AddPlayer(playerId);
}

bool LobbyManager::RemovePlayerFromLobby(int lobbyId, int playerId) {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false;
    }

    it->second->RemovePlayer(playerId);

    // If the lobby becomes empty, delete it automatically
    if (it->second->GetPlayers().empty()) {
        m_lobbies.erase(it);
    }

    return true;
}

std::vector<int> LobbyManager::GetActiveLobbyIds() const {
    std::lock_guard<std::mutex> lock(m_lobbyMutex);

    std::vector<int> lobbyIds;
    lobbyIds.reserve(m_lobbies.size()); // Reserve space for efficiency

    std::transform(m_lobbies.begin(), m_lobbies.end(), std::back_inserter(lobbyIds),
        [](const auto& pair) { return pair.first; }); // Extract keys

    return lobbyIds;
}
