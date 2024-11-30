#pragma once

#include "Vector2.h"

#include <QtGui\QPainter>
#include <QtGui\QBrush>
#include <QtGui\QPen>
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(const Vector2& position, const Vector2& direction, float speed, float damage);

	Bullet(Bullet&& other) noexcept;
	Bullet& operator=(Bullet&& other) noexcept;

	Bullet(const Bullet& other) = delete;
	Bullet& operator=(const Bullet& other) = delete;

	~Bullet() = default;


	void Update();
	void draw(QPainter& painter) const;

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

