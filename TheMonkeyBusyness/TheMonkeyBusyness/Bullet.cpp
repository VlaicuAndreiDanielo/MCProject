#include "Bullet.h"

Bullet::Bullet(const Vector2<float>& position, const Vector2<float>& direction, float speed, float damage)
	: m_position{ position }, m_direction{ direction }, m_speed{ speed }, m_damage{ damage } {}

Bullet::Bullet(Bullet&& other) noexcept
	: m_position(std::move(other.m_position)),
	m_direction(std::move(other.m_direction)),
	m_speed(other.m_speed),
	m_damage(other.m_damage) {}

Bullet& Bullet::operator=(Bullet&& other) noexcept
{
	if (this != &other)
	{
		m_position = std::move(other.m_position);
		m_direction = std::move(other.m_direction);
		m_speed = other.m_speed;
		m_damage = other.m_damage;
	}
	return *this;
}

void Bullet::Update(float deltaTime)
{
	m_position.x += m_direction.x * m_speed * deltaTime;
	m_position.y += m_direction.y * m_speed * deltaTime;
}

crow::json::wvalue Bullet::ToJson() const
{
	crow::json::wvalue bulletJson;
	bulletJson["x"] = m_position.x;
	bulletJson["y"] = m_position.y;
	bulletJson["directionX"] = m_direction.x;
	bulletJson["directionY"] = m_direction.y;
	return bulletJson;
}

Vector2<float> Bullet::GetPosition() const {
	return m_position;
}

void Bullet::SetPosition(const Vector2<float>& position) {
	m_position = position;
}

Vector2<float> Bullet::GetDirection() const {
	return m_direction;
}

void Bullet::SetDirection(const Vector2<float>& direction) {
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