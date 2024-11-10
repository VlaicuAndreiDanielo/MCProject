#include "Bullet.h"

Bullet::Bullet(const Vector2& position, const Vector2& direction, float speed, float damage)
	: m_position{ position }, m_direction{ direction }, m_speed{ speed }, m_damage{ damage } {}

void Bullet::Update() {
	// TODO: uncomment the next lines after overloading += in Vector2 class
	Vector2 newPosition = m_position;
	//newPosition += GetDirection() * GetSpeed();

	SetPosition(newPosition);
}

bool Bullet::CheckCollision(const Vector2& enemyPosition, float enemyRadius) const {
	// TODO: uncomment the next two lines after overloading - and adding a method for getting the length of a vector
	//float distance = (enemyPosition - m_position).Length()
	//return distance < enemyRadius;
	return true;  //temporary line
}

Vector2 Bullet::GetPosition() const {
	return m_position;
}

void Bullet::SetPosition(const Vector2& position) {
	// TODO: after finishing map logic, implement checks so the bullet position can't be set in wrong places like outside of the map
	m_position = position;
}

Vector2 Bullet::GetDirection() const {
	return m_direction;
}

void Bullet::SetDirection(const Vector2& direction) {
	m_direction = direction;
}

float Bullet::GetSpeed() const {
	return m_speed;
}

void Bullet::SetSpeed(float speed) {
	m_speed = speed;
}

float Bullet::GetDamage() const {
	return m_damage;
}

void Bullet::SetDamage(float damage) {
	m_damage = damage;
}