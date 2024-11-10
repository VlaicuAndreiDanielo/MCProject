#pragma once

#include "Vector2.h"

class Bullet
{
public:
	Bullet(const Vector2& position, const Vector2& direction, float speed, float damage);

	void Update();
	bool CheckCollision(const Vector2& enemyPosition, float enemyRadius) const;

	Vector2 GetPosition() const;
	Vector2 GetDirection() const;
	float GetSpeed() const;
	float GetDamage() const;

	void SetPosition(const Vector2& position);
	void SetDirection(const Vector2& direction);
	void SetSpeed(float speed);
	void SetDamage(float damage);

private:
	Vector2 m_position;
	Vector2 m_direction;
	float m_speed;
	float m_damage;
};

