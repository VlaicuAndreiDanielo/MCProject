#pragma once

#include "Vector2.h"

#include <QtGui\QPainter>  //TODO temporary for draw methods
#include <QtGui\QBrush>  //TODO temporary for draw methods
#include <QtGui\QPen>  //TODO temporary for draw methods


class Bullet
{
public:
	Bullet(const Vector2& position, const Vector2& direction, float speed, float damage);

	Bullet(Bullet&& other) noexcept;
	Bullet& operator=(Bullet&& other) noexcept;

	Bullet(const Bullet& other) = default;     //TODO think if I should leave the default or delete it
	Bullet& operator=(const Bullet& other) = default; //TODO maybe I don't want to allow copying of bullets

	~Bullet() = default;


	void Update();
	bool CheckCollision(const Vector2& enemyPosition, float enemyRadius) const;
	void draw(QPainter& painter) const;  //TODO temporary for draw methods

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

