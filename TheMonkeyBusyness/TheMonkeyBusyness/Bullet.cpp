#include "Bullet.h"

Bullet::Bullet(const Vector2& position, const Vector2& direction, float speed, float damage)
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

void Bullet::Update()
{
	// TODO: uncomment the next lines after overloading += in Vector2 class
	Vector2 newPosition = m_position;
	newPosition.x += m_direction.x * m_speed;
	newPosition.y += m_direction.y * m_speed;

	SetPosition(newPosition);
}

void Bullet::draw(QPainter& painter) const { //TODO temporary for draw methods
	painter.save();
	QPen pen(Qt::black);
	QBrush brush(Qt::red);

	painter.setPen(pen);
	painter.setBrush(brush);

	const float bulletRadius = 5.0f;

	painter.drawEllipse(QPointF(m_position.x, m_position.y), bulletRadius, bulletRadius);
	painter.restore();
}


bool Bullet::CheckCollision(const Vector2& enemyPosition, float enemyRadius) const
{
	// TODO: uncomment the next two lines after overloading - and adding a method for getting the length of a vector
	//float distance = (enemyPosition - m_position).Length()
	//return distance < enemyRadius;
	return true;  //temporary line
}

Vector2 Bullet::GetPosition() const {
	return m_position;
}

void Bullet::SetPosition(const Vector2& position) {
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