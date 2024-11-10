#include "Weapon.h"

Weapon::Weapon(float damage, float fireRate, float speed)
	: m_damage{ damage }, m_fireRate{ fireRate }, m_speed{ speed }, m_timeSinceLastShot(0.0f) {}

//
void Weapon::Shoot(const Vector2& position, const Vector2& direction)
{
	/*
	TODO

	Bullets are stored in a vector in the weapon class
	Now the bullets are created and deleted very often.
	To make this more efficient, create a lot of bullets and use them instead of creating and deleting constantly
	- either have two vectors: unusedBullets and firedBullets and use move semantics to efficiently "shoot" bullets
	- or have one vector and add a flag to each bullet like isAlive. When the weapon fires, just set the bullet flag to true

	I think having two vectors and using move semantics is better, but check when implementing that.
	The vector/vectors will be stored in the player class
	*/
}

void Weapon::Update()
{
	m_timeSinceLastShot += 1.0f / 60.0f;
}

float Weapon::GetDamage() const {
	return m_damage;
}

void Weapon::SetDamage(float damage) {
	m_damage = damage;
}

float Weapon::GetFireRate() const {
	return m_fireRate;
}

void Weapon::SetFireRate(float fireRate) {
	m_fireRate = fireRate;
}

float Weapon::GetSpeed() const {
	return m_speed;
}

void Weapon::SetSpeed(float speed) {
	m_speed = speed;
}
