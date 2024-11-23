#pragma once
#include "Vector2.h"
#include "iostream"
#include "Player.h"
#include "Arena.h"
#include "Tile.h"
#include "ConstantValues.h"
using HitResult = std::variant<Tile, Player>;
class Cast {
public:
	static HitResult Raycast(Vector2 origin, Vector2 direction, float maxDistance);
	
};