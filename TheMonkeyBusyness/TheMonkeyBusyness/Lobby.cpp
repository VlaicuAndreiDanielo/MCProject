#include "Lobby.h"
#include <stdexcept>
#include "ConstantValues.h"

Lobby::Lobby(int lobbyId, int hostId)
    : lobbyId(lobbyId), hostId(hostId), status(LobbyStatus::Waiting) {
    players[hostId] = false; // Host is initially not ready
}

bool Lobby::addPlayer(int playerId) {
    if (status != LobbyStatus::Waiting || players.size() >= GameConfig::kMaxLobbyPlayers) {
        return false; // Lobby is full or already in-game
    }
    players[playerId] = false;
    return true;
}

bool Lobby::removePlayer(int playerId) {
    if (players.find(playerId) == players.end()) {
        return false; // Player not in lobby
    }

    players.erase(playerId);

    // If host leaves, assign a new host
    if (playerId == hostId && !players.empty()) {
        hostId = players.begin()->first;
    }

    return true;
}

void Lobby::setReady(int playerId, bool isReady) {
    if (players.find(playerId) != players.end()) {
        players[playerId] = isReady;
    }
}

bool Lobby::isAllReady() const {
    for (const auto& [playerId, isReady] : players) {
        if (!isReady) {
            return false;
        }
    }
    return true;
}

bool Lobby::startGame(int& gameId) {
    if (status != LobbyStatus::Waiting || !hasMinimumPlayers() || !isAllReady()) {
        return false; // Can't start game
    }

    status = LobbyStatus::InGame;
    gameId = lobbyId; // Example: Use lobbyId as gameId for simplicity
    return true;
}

void Lobby::resetLobby() {
    status = LobbyStatus::Waiting;
    for (auto& [playerId, isReady] : players) {
        isReady = false;
    }
}

int Lobby::getLobbyId() const {
    return lobbyId;
}

int Lobby::getHostId() const {
    return hostId;
}

LobbyStatus Lobby::getStatus() const {
    return status;
}

std::unordered_map<int, bool> Lobby::getPlayers() const {
    return players;
}

bool Lobby::hasMinimumPlayers() const {
    return players.size() >= GameConfig::kMinLobbyPlayers;
}
