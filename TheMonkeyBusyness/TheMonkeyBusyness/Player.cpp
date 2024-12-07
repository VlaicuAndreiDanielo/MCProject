#include "Player.h"
#include "iostream"

Player::Player(float x, float y, int id) : m_id{ PlayerConfig::kDefaultPlayeId } //sets spawn of the player 
{
	m_Character = new BasicMonkey();
	m_position.x = x;
	m_position.y = y;
	m_id = id;
}

Vector2 Player::Forward()
const {
	return this->m_direction;
}

Vector2 Player::GetPosition() const
{
	return m_position;
}

void Player::SetSpawn(Vector2 location)
{
	m_position = location;
}

void Player::SetMonkeyType(Character* character) 
{
	m_Character = character;
}

void Player::UpdatePosition(const Vector2& direction)
{
	this->m_position.x += direction.x * m_Character->getSpeed();
	this->m_position.y += direction.y * m_Character->getSpeed();
}

void Player::UpdatePosition(const float x, const float y)
{
	this->m_position.x += x;
	this->m_position.y += y;
}

void Player::UpdateRotation(const Vector2& mousePos)
{
	this->m_rotationAngle = CalculateLookAtDirection(mousePos).GetAngleFromNormalizedVector();
}

void Player::Shoot(const Vector2& mousePosition)
{
	// Update rotation based on current mouse position
	m_rotationAngle = CalculateLookAtDirection(mousePosition).GetAngleFromNormalizedVector();

	// Calculate bullet spawn position and direction
	Vector2 bulletSpawnPosition = CalculateBulletSpawnPosition();
	Vector2 shootDirection = CalculateLookAtDirection(mousePosition);

	// Log player position, bullet position, and direction
	std::ofstream logFile("server_log.txt", std::ios::app);
	logFile << "Player " << m_id << " shooting:\n"
		<< "  Player Position: (" << m_position.x << ", " << m_position.y << ")\n"
		<< "  Bullet Spawn Position: (" << bulletSpawnPosition.x << ", " << bulletSpawnPosition.y << ")\n"
		<< "  Bullet Direction: (" << shootDirection.x << ", " << shootDirection.y << ")\n";

	// Shoot logic (e.g., add bullet to game state)
	m_weapon.Shoot(bulletSpawnPosition, shootDirection);
}


void Player::Update(float deltaTime)
{
	// handles updating the timers on the powerup cooldowns

	std::ofstream logFile("server_log.txt", std::ios::app);
	logFile << "Player " << m_id << " Update called with deltaTime=" << deltaTime << std::endl;

	m_weapon.Update(deltaTime);
}

bool Player::IsAlive() const
{
	return m_Character->getHealth() > 0;
}

void Player::SetScreenSize(const int screenWidth, const int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

crow::json::wvalue Player::toJson() const
{
	crow::json::wvalue playerJson;
	playerJson["id"] = m_id;
	playerJson["x"] = m_position.x;
	playerJson["y"] = m_position.y;
	playerJson["hp"] = m_Character->getHealth();

	playerJson["weapon"] = m_weapon.toJson();

	return playerJson;
}


//Vector2 Player::CalculateLookAtDirection(const Vector2& mousePos)
//{
//	int mouseOffsetX = mousePos.x - (m_screenWidth / 2 - m_position.x);
//	int mouseOffsetY = mousePos.y - (m_screenHeight / 2 - m_position.y);
//	m_direction.x = mouseOffsetX - m_position.x;
//	m_direction.y = mouseOffsetY - m_position.y;
//	m_direction.Normalize();
//	return m_direction;
//}

Vector2 Player::CalculateLookAtDirection(const Vector2& mousePos)
{
	// Calculate direction vector from player's position to mouse position
	Vector2 direction = mousePos - m_position;

	// Normalize the direction vector to make it a unit vector
	direction.Normalize();

	// Set player's direction and return it
	m_direction = direction;
	return m_direction;
}


Vector2 Player::CalculateBulletSpawnPosition() const
{
	float offsetX = cos((m_rotationAngle - 90) * PI / 180.0f) * (kPlayerSize / 2.0f);
	float offsetY = sin((m_rotationAngle - 90) * PI / 180.0f) * (kPlayerSize / 2.0f);

	return m_position + Vector2{ offsetX, offsetY };
}

