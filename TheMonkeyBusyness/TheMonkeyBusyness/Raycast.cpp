#include "Raycast.h"
#include "iostream"
#include "math.h"
HitResult Cast::Raycast(Vector2 origin, Vector2 direction, float maxDistance)
{
	Vector2 hitLocation = origin + direction * maxDistance;
	//TO DO: Cycle through all existing player positions and check if hitLocation is near any of them, and if so return a reference to the player
	hitLocation /= SQUARE_SIZE;
	hitLocation.x = std::floor(hitLocation.x);
	hitLocation.y = std::floor(hitLocation.y);
	return HitResult(std::move(Arena::GetTile(hitLocation.y, hitLocation.x)));
	
}
