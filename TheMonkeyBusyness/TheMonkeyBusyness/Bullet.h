#pragma once

#include "Vector2.h"
#include "GameObject.h"
#include <crow.h>

class Bullet : public GameObject
{
public:
	Bullet(const Vector2<float>& position, const Vector2<float>& direction, float speed, float damage);

	Bullet(Bullet&& other) noexcept;
	Bullet& operator=(Bullet&& other) noexcept;

	Bullet(const Bullet& other) = default;  
	Bullet& operator=(const Bullet& other) = default; 

	~Bullet() = default;

	void Update(float deltaTime);
	crow::json::wvalue ToJson() const;

	Vector2<float> GetPosition() const;
	Vector2<float> GetDirection() const;
	float GetSpeed() const;
	float GetDamage() const;

	void SetPosition(const Vector2<float>& position);
	void SetDirection(const Vector2<float>& direction);
	void SetSpeed(float speed);
	void SetDamage(float damage);

private:
	Vector2<float> m_position;
	Vector2<float> m_direction;
	float m_speed;
	float m_damage;
};

