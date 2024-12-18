#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class Lobby {
public:
    explicit Lobby(int lobbyId, int hostId);

    // Player Management
    bool AddPlayer(int playerId);
    bool RemovePlayer(int playerId);
    void SetReady(int playerId, bool isReady);
    bool IsAllReady() const;

    // Game Management
    bool HasMinimumPlayers() const;
    bool HasMaximumPlayers() const;

    // Accessors
    int GetLobbyId() const;
    int GetHostId() const;
    std::unordered_map<int, bool> GetPlayers() const;

private:
    int m_lobbyId;                    
    int m_hostId;                   
    std::unordered_map<int, bool> m_players; // Players and their ready status

};
