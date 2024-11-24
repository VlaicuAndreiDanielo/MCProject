#pragma once
#include "InputHandler.h"
#include <QtCore/qobject.h>
#include <QtGui/qpainter.h>
#include "Weapon.h"
#include "ConstantValues.h"

class Player : public QObject{ // this is the player, he calls for input and other actions
public:
	explicit Player(float x=0, float y=0);
	InputHandler* ReturnInputHandler();
	Vector2 Forward() const;
	void draw(QPainter& painter)const;
	Vector2 GetPosition()const;
	void SetSpawn(Vector2 location);
	Weapon weapon;
public slots:
	void UpdatePosition(const Vector2 &vector);
	void UpdatePosition(const float x, const float y);
	void UpdateRotation(const Vector2& mousePos, const int screenW, const int screenH);
	void Shoot(const Vector2& mousePos, const int screenW, const int screenH);
private:
	InputHandler inputHandler;
	Vector2 position;
	Vector2 direction;
	float rotationAngle{ 0 };
	float size{ kPlayerSize };
	Vector2 CalculateLookAtDirection(const Vector2& mousePos, const int screenW, const int screenH);
	Vector2 CalculateBulletSpawnPosition() const;
};