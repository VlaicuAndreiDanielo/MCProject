#include "Raycast.h"
#include "iostream"
#include "math.h"
#include "ConstantValues.h"
GameObject* Cast::Raycast(Vector2<float> origin, Vector2<float> direction, float maxDistance, Player& sender)
{
	Vector2 hitLocation = origin + direction * maxDistance;

	//TO DO: Cycle through all existing player positions and check if hitLocation is near any of them, and if so return a reference to the player
	
	if (HandlePlayerHit(hitLocation, sender) != nullptr) {
		return HandlePlayerHit(hitLocation, sender);
	}

	return HandleTileHit(hitLocation);
}

GameObject* Cast::Raycast(Vector2<float> origin, Vector2<float> direction, float maxDistance, Player& sender, Vector2<float>& CastResult)
{
	Vector2<float> hitLocation = origin + direction * maxDistance;
	CastResult = hitLocation;
	if (HandlePlayerHit(hitLocation, sender) != nullptr) {
		return HandlePlayerHit(hitLocation, sender);
	}
	return HandleTileHit(hitLocation);
}

GameObject* Cast::HandleTileHit(Vector2<float>& hitLocation)
{
	hitLocation /= GameConfig::kTileSize;
	hitLocation.x = std::floor(hitLocation.x);
	hitLocation.y = std::floor(hitLocation.y);
	GameObject* gameTile = &m_arena->GetTile(hitLocation.y, hitLocation.x);
	return gameTile;
}

GameObject* Cast::HandlePlayerHit(Vector2<float>& hitLocation, Player& sender)
{
	int senderId=-1;
	for (auto& [id, player] : *m_players) {
		if (sender.GetPosition() == player.GetPosition()) {
			senderId = id;
		}
	}
	for (auto& [id, player] : *m_players) {
		Vector2<float> playerPosition = player.GetPosition();
		float dx = hitLocation.x - playerPosition.x;
		float dy = hitLocation.y - playerPosition.y;
		float distanceSquared = dx * dx + dy * dy;
		if (distanceSquared <= PlayerConfig::kPlayerSize * PlayerConfig::kPlayerSize && id!=senderId) {
			GameObject* playerRef = &player;
			return playerRef;
		}
	}
	return nullptr;
}


