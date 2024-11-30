#pragma once
#include "InputHandler.h"
#include <QtCore/qobject.h>
#include <QtGui/qpainter.h>
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
class Player : public GameObject { // this is the player, he calls for input and other actions
public:
	explicit Player(float x=0, float y=0);
	InputHandler* ReturnInputHandler();
	Vector2 Forward() const;
	void draw(QPainter& painter)const;
	Vector2 GetPosition()const;
	void SetSpawn(Vector2 location);
	void SetMonkeyType(Character* character);
	Weapon m_weapon;
public slots:
	void UpdatePosition(const Vector2 &vector);
	void UpdatePosition(const float x, const float y);
	void UpdateRotation(const Vector2& mousePos, const int screenW, const int screenH);
	void Shoot(const Vector2& mousePos, const int screenW, const int screenH);
private:
	Character* m_Character;
	InputHandler m_inputHandler;
	Vector2 m_position;
	Vector2 m_direction;
	float m_rotationAngle{ 0 };
	float m_size{ kPlayerSize };
	Vector2 CalculateLookAtDirection(const Vector2& mousePos, const int screenW, const int screenH);
	Vector2 CalculateBulletSpawnPosition() const;
};