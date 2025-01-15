#pragma once
#include "Weapon.h"
#include "ConstantValues.h"
#include "Character.h"
#include "AnubisBaboon.h"
#include "BasicMonkey.h"
#include "HowlerMonkey.h"
#include "Orangutan.h"
#include "CapuchinMonkey.h"
#include "Gorilla.h"
#include "GameObject.h"
#include "ConstantValues.h"

class Player : public GameObject { // this is the player, he calls for input and other actions
public:
	explicit Player(float x = PlayerConfig::kDefaultPositionX, float y = PlayerConfig::kDefaultPositionY, int id = PlayerConfig::kDefaultPlayerId, const std::string& name = PlayerConfig::kDefaultPlayerName);
	Vector2<float> Forward() const;
	Vector2<float> GetPosition()const;
	void SetSpawn(Vector2<float> location);
	void SetMonkeyType(Character* character);
	Weapon m_weapon;

	void UpdatePosition(const Vector2<float> &vector, float deltaTime);
	void UpdatePosition(const float x, const float y);
	void UpdateRotation(const Vector2<float>& mousePos);
	void Shoot(const Vector2<float>& mousePosition);
	void Update(float deltaTime);
	bool IsAlive() const;
	void Damage(int damageValue);
	void SetScreenSize(const int screenWidth, const int screenHeight);

	const std::string& GetName() const;

	crow::json::wvalue ToJson() const;
private:
	int m_id;
	std::string m_name;
	Character* m_Character;
	Vector2<float> m_position;
	Vector2<float> m_direction;
	float m_rotationAngle{ 0 };
	float m_size{ PlayerConfig::kPlayerSize };
	int m_screenWidth;
	int m_screenHeight;
	Vector2<float> CalculateLookAtDirection(const Vector2<float>& mousePos);
	Vector2<float> CalculateBulletSpawnPosition() const;
};