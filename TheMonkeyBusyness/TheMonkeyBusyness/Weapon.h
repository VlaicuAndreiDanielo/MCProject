#pragma once

#include "Vector2.h";
#include "Bullet.h";

class Weapon
{
public:
	Weapon(float damage, float fireRate, float speed);

	void Shoot(const Vector2& position, const Vector2& direction);
	void Update();

	float GetDamage() const;
	float GetFireRate() const;
	float GetSpeed() const;

	void SetDamage(float damage);
	void SetFireRate(float fireRate);
	void SetSpeed(float speed);

private:
	float m_damage;
	float m_fireRate;
	float m_speed;

	float m_timeSinceLastShot;
};