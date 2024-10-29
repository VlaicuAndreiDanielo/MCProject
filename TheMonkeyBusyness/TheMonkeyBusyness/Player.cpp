#include "Player.h"
#include "iostream"
Player::Player(QObject* parent) : QObject(parent)
{
}
InputHandler* Player::ReturnInputHandler()
{
	return &(this->inputHandler);
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
