#pragma once

#include "Vector2.h"
#include "GameObject.h"
#include <crow.h>

class Bullet : public GameObject
{
public:
	Bullet(const Vector2& position, const Vector2& direction, float speed, float damage);

	Bullet(Bullet&& other) noexcept;
	Bullet& operator=(Bullet&& other) noexcept;

	Bullet(const Bullet& other) = default;  //TODO check if I can delete it
	Bullet& operator=(const Bullet& other) = default; //TODO check if I can delete it

	~Bullet() = default;

	void Update(float deltaTime);
	crow::json::wvalue toJson() const;

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

