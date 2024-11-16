#pragma once

#include "Vector2.h"
#include "Bullet.h"
#include <vector>

class Weapon
{
public:
	Weapon(float damage=10, float fireRate=1, float speed=1);

	void Shoot(const Vector2& position, const Vector2& direction);
	void Update();
	void DrawBullets(QPainter& painter) const; //TODO temporary for draw methods

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
	std::vector<Bullet> m_activeBullets;
	std::vector<Bullet> m_inactiveBullets;
};