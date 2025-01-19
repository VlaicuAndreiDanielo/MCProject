#include "Player.h"
#include "iostream"

Player::Player(float x, float y, int id, const std::string& name)
	: m_id{ id }, m_name{ name }, m_position{ x, y }
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> distrib(0, 3);
	m_monkeyType = distrib(gen);
	
	// Alege caracterul pe baza valorii random
	switch (m_monkeyType) {
	case 0:
		m_Character = new BasicMonkey();
		std::cout << "BasicMonkey selected for " << name << std::endl;
		break;
	case 1:
		m_Character = new CapuchinMonkey();
		std::cout << "CapuchinMonkey selected for " << name << std::endl;
		break;
	case 2:
		m_Character = new Gorilla();
		std::cout << "Gorilla selected for " << name << std::endl;
		break;
	case 3:
		m_Character = new Orangutan();
		std::cout << "Orangutan selected for " << name << std::endl;
		break;
	default:
		m_Character = new BasicMonkey();
		std::cout << "Default BasicMonkey selected for " << name << std::endl;
		break;
	}
}

Vector2<float> Player::Forward()
const {
	return this->m_direction;
}

Vector2<float> Player::GetPosition() const
{
	return m_position;
}

Character* Player::GetCharacter() const {
	return m_Character;
}

void Player::SetSpawn(Vector2<float> location)
{
	m_position = location;
}

void Player::SetMonkeyType(Character* character) 
{
	m_Character = character;
}

void Player::SetOldSpeed(int speed)
{
	m_oldSpeed = speed;
}

int Player::GetOldSpeed() const 
{
	return m_oldSpeed;
}

void Player::SetisSlowed(bool isSlowed)
{
	m_isSlowed = isSlowed;
}

bool Player::IsSlowed() const {
	return m_isSlowed;
}

void Player::StartDoT(float durationInSeconds) {
	if (m_isUnderDot) return; // DoT already active
	m_isUnderDot = true;
	m_dotStartTime = std::chrono::steady_clock::now();
	m_dotDuration = durationInSeconds;
}

void Player::StopDoT() {
	m_isUnderDot = false;
	m_dotDuration = 0;
}

bool Player::IsUnderDot() const {
	return m_isUnderDot;
}

void Player::UpdateDot() {
	if (!m_isUnderDot) return;

	auto now = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(now - m_dotStartTime).count();

	if (elapsedTime >= m_dotDuration) {
		StopDoT();
		return;
	}

	static float timeSinceLastTick = 0.0f;
	timeSinceLastTick += elapsedTime;

	if (timeSinceLastTick >= 0.3f) { // Tick every 0.3 seconds
		Damage(5);
		timeSinceLastTick = 0.0f;
	}

	m_dotStartTime = now; //Resets reference time
}

void Player::UpdatePosition(const Vector2<float>& direction, float deltaTime)
{
	Vector2<float> normalizedDir = Vector2<float>::Normalize(direction);
	
	this->m_position.x += normalizedDir.x * m_Character->GetSpeed() * deltaTime;
	this->m_position.y += normalizedDir.y * m_Character->GetSpeed() * deltaTime;
}

void Player::UpdatePosition(const float x, const float y)
{
	this->m_position.x += x;
	this->m_position.y += y;
}

void Player::UpdateRotation(const Vector2<float>& mousePos)
{
	this->m_rotationAngle = CalculateLookAtDirection(mousePos).GetAngleFromNormalizedVector();
}

void Player::Shoot(const Vector2<float>& mousePosition)
{
	// Update rotation based on current mouse position
	m_rotationAngle = CalculateLookAtDirection(mousePosition).GetAngleFromNormalizedVector();

	// Calculate bullet spawn position and direction
	Vector2 bulletSpawnPosition = CalculateBulletSpawnPosition();
	Vector2 shootDirection = CalculateLookAtDirection(mousePosition);

	// Shoot logic (e.g., add bullet to game state)
	m_weapon.Shoot(bulletSpawnPosition, shootDirection);
}


void Player::Update(float deltaTime)
{
	// handles updating the timers on the powerup cooldowns
	m_weapon.Update(deltaTime);
}

bool Player::IsAlive()
{
	if (m_Character->GetHealth() < 0)
	{
		m_isAlive = 0;
		return false;
	}
	else
	{
		m_isAlive = 1;
		return true;
	}
}

void Player::Damage(int damageValue)
{
	if (m_Character != nullptr) {
		int hp = m_Character->GetHealth();
		m_Character->SetHealth(hp - damageValue);
	}
}

void Player::ActivateAbility()
{
	m_Character->ActivateSpecialAbility();
}

void Player::SetScreenSize(const int screenWidth, const int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

const std::string& Player::GetName() const { return m_name; }

crow::json::wvalue Player::ToJson() const
{
	crow::json::wvalue playerJson;
	playerJson["id"] = m_id;
	playerJson["name"] = m_name;
	playerJson["x"] = m_position.x;
	playerJson["y"] = m_position.y;
	playerJson["directionX"] = m_direction.x;
	playerJson["directionY"] = m_direction.y;
	playerJson["hp"] = m_Character->GetHealth();
	playerJson["monkeyType"] = m_monkeyType;
	playerJson["weapon"] = m_weapon.ToJson();
	playerJson["isAlive"] = m_isAlive;

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
// ^ that was needed

Vector2<float> Player::CalculateLookAtDirection(const Vector2<float>& mousePos)
{
	int mouseOffsetX = mousePos.x - (m_screenWidth / 2 - m_position.x); //nu stiu daca tragi screen size din client sau nu deci voi folosii valorile actuale
		int mouseOffsetY = mousePos.y - (m_screenHeight / 2 - m_position.y); //nu stiu daca tragi screen size din client sau nu deci voi folosii valorile actuale
		m_direction.x = mouseOffsetX - m_position.x;
		m_direction.y = mouseOffsetY - m_position.y;
		m_direction.Normalize();
		return m_direction;
}


Vector2<float> Player::CalculateBulletSpawnPosition() const
{
	float offsetX = cos((m_rotationAngle - GameConfig::kDefaultRotationOffset) * MathConfig::kPi / 180.0f) * (PlayerConfig::kPlayerSize / 2.0f);
	float offsetY = sin((m_rotationAngle - GameConfig::kDefaultRotationOffset) * MathConfig::kPi / 180.0f) * (PlayerConfig::kPlayerSize / 2.0f);

	return m_position + Vector2<float>{ offsetX, offsetY };
}

