#include "Player.h"
#include "iostream"
Player::Player(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
}
InputHandler* Player::ReturnInputHandler()
{
	return &(this->inputHandler);
}

float Player::Forward()
const {
	return rotationAngle;
}

void Player::draw(QPainter& painter) const
{
	painter.save();

	// Translate and rotate to draw the player at the correct position and angle
	painter.translate(position.x, position.y);
	painter.rotate(rotationAngle);

	// Draw the player as a square centered on the origin
	painter.drawRect(-size / 2, -size / 2, size, size);

	painter.restore();
}

void Player::UpdatePosition(const Vector2& direction)
{
	this->position.x += direction.x;
	this->position.y += direction.y;
	std::cout << position;
}

void Player::UpdatePosition(const float x, const float y)
{
}
