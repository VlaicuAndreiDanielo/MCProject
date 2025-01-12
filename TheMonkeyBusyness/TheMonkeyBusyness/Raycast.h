#pragma once
#include "Vector2.h"
#include "iostream"
#include "Arena.h"
#include "Tile.h"
#include "ConstantValues.h"
#include "Player.h"

class Cast {
public:
    GameObject* Raycast(Vector2 origin, Vector2 direction, float maxDistance, Player &sender);
	GameObject* Raycast(Vector2 origin, Vector2 direction, float maxDistance, Player &sender, Vector2& CastResult);
	Arena* m_arena;
	std::unordered_map<int, Player>* m_players;
private:
	GameObject* HandleTileHit(Vector2 &hitLocation);
	GameObject* HandlePlayerHit(Vector2& hitLocation, Player &sender);
};