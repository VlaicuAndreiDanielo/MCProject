#pragma once
#include "Vector2.h"
#include "iostream"
#include "Arena.h"
#include "Tile.h"
#include "ConstantValues.h"
#include "Player.h"
#include <memory>
#include <unordered_map>

class Cast {
public:
    GameObject* Raycast(Vector2<float> origin, Vector2<float> direction, float maxDistance, Player& sender);
    GameObject* Raycast(Vector2<float> origin, Vector2<float> direction, float maxDistance, Player& sender, Vector2<float>& CastResult);
    std::weak_ptr<Arena> m_arena;
    std::weak_ptr<std::unordered_map<int, Player>> m_players;
private:
    GameObject* HandleTileHit(Vector2<float>& hitLocation);
    GameObject* HandlePlayerHit(Vector2<float>& hitLocation, Player& sender);
};
