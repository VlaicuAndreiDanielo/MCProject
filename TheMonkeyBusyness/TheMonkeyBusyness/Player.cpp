#include "Player.h"
#include "iostream"

Player::Player(float x, float y) //sets spawn of the player 
{
	m_Character = new BasicMonkey();
	m_position.x = x;
	m_position.y = y;
}


Vector2 Player::Forward()
const {
	return this->m_direction;
}

//void Player::draw(QPainter& painter) const
//{
//	painter.save();
//
//	// Translate and rotate to draw the player at the correct position and angle
//	painter.translate(m_position.x, m_position.y);
//	painter.rotate(m_rotationAngle);
//
//	painter.setBrush(Qt::white);
//	QRect rectangle(-m_size / 2, -m_size / 2, m_size, m_size); // x, y, width, height
//	painter.fillRect(rectangle, painter.brush()); // Fill the rectangle
//
//	// Draw the player as a square centered on the origin
//	painter.setPen(Qt::white);
//	painter.drawRect(rectangle);
//
//	painter.restore();
//}

Vector2 Player::GetPosition() const
{
	return m_position;
}

void Player::SetSpawn(Vector2 location)
{
	m_position = location;
}

void Player::SetMonkeyType(Character* character) 
{
	m_Character = character;
}

void Player::UpdatePosition(const Vector2& direction)
{
	this->m_position.x += direction.x * m_Character->getSpeed();
	this->m_position.y += direction.y * m_Character->getSpeed();
	//std::cout << position;
}

void Player::UpdatePosition(const float x, const float y)
{
	this->m_position.x += x;
	this->m_position.y += y;
}

void Player::UpdateRotation(const Vector2& mousePos, const int screenW, const int screenH)
{
	
	this->m_rotationAngle = CalculateLookAtDirection(mousePos, screenW, screenH).GetAngleFromNormalizedVector();
	//std::cout << rotationAngle << std::endl;
}

void Player::Shoot(const Vector2& mousePosition, const int screenW, const int screenH)
{
	//Mario aici trebuie returnat input de la sever pt daca trage sau nu
	//if (m_inputHandler.is_shooting)
	{
		Vector2 bulletSpawnPosition = CalculateBulletSpawnPosition();
 		Vector2 shootDirection = CalculateLookAtDirection(mousePosition, screenW, screenH);

		m_weapon.Shoot(bulletSpawnPosition, shootDirection);
	}
	m_weapon.Update();
}


Vector2 Player::CalculateLookAtDirection(const Vector2& mousePos, const int screenW, const int screenH)
{
	int mouseOffsetX = mousePos.x - (screenW / 2 - m_position.x);
	int mouseOffsetY = mousePos.y - (screenH / 2 - m_position.y);
	m_direction.x = mouseOffsetX - m_position.x;
	m_direction.y = mouseOffsetY - m_position.y;
	m_direction.Normalize();
	return m_direction;
}

Vector2 Player::CalculateBulletSpawnPosition() const
{
	float offsetX = cos((m_rotationAngle - 90) * PI / 180.0f) * (kPlayerSize / 2.0f);
	float offsetY = sin((m_rotationAngle - 90) * PI / 180.0f) * (kPlayerSize / 2.0f);

	return m_position + Vector2{ offsetX, offsetY };
}

