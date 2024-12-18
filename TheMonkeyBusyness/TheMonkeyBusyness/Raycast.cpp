#include "Raycast.h"
#include "iostream"
#include "math.h"
GameObject* Cast::Raycast(Vector2 origin, Vector2 direction, float maxDistance)
{
	Vector2 hitLocation = origin + direction * maxDistance;
	//TO DO: Cycle through all existing player positions and check if hitLocation is near any of them, and if so return a reference to the player
	hitLocation /= GameConfig::kTileSize;
	hitLocation.x = std::floor(hitLocation.x);
	hitLocation.y = std::floor(hitLocation.y);
	GameObject* gameTile = &m_arena->GetTile(hitLocation.y, hitLocation.x);
	return gameTile;
	
}
