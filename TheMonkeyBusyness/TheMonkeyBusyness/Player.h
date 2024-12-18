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
	explicit Player(float x=0, float y=0, int id = -1);
	Vector2 Forward() const;
	Vector2 GetPosition()const;
	void SetSpawn(Vector2 location);
	void SetMonkeyType(Character* character);
	Weapon m_weapon;

	void UpdatePosition(const Vector2 &vector);
	void UpdatePosition(const float x, const float y);
	void UpdateRotation(const Vector2& mousePos);
	void Shoot(const Vector2& mousePosition);
	void Update(float deltaTime);
	bool IsAlive() const;

	void SetScreenSize(const int screenWidth, const int screenHeight);

	crow::json::wvalue ToJson() const;
private:
	int m_id;
	Character* m_Character;
	Vector2 m_position;
	Vector2 m_direction;
	float m_rotationAngle{ 0 };
	float m_size{ kPlayerSize };
	int m_screenWidth;
	int m_screenHeight;
	Vector2 CalculateLookAtDirection(const Vector2& mousePos);
	Vector2 CalculateBulletSpawnPosition() const;
};