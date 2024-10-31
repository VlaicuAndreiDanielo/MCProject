#include "Vector2.h"
#include "cmath"
#include <stdexcept>
Vector2::Vector2(float x, float y) : x(x), y(y)
{}

Vector2 Vector2::Normalize(const Vector2 vector2)
{
	float magnitude = (float) std::sqrt(pow(vector2.x,2) + pow(vector2.y,2));
	if (magnitude == 0) return Vector2();
	return Vector2(vector2.x / magnitude, vector2.y / magnitude);
}

void Vector2::Normalize()
{
	float magnitude = (float) std::sqrt(pow(this->x, 2) + pow(this->y, 2));
	if (!magnitude == 0)  (*this /= magnitude);
}

Vector2 Vector2::operator*(const Vector2& vector) const
{
	return Vector2(this->x * vector.x, this->y * vector.y);
}

Vector2 Vector2::operator*(const float scalar) const
{
	return Vector2(this->x*scalar,this->y*scalar);
}

Vector2& Vector2::operator*=(const float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	return *this;
}



Vector2 Vector2::operator/(const float scalar) const {

	if (scalar != 0) {
		return Vector2(this->x / scalar, this->y / scalar);
	}
	else {
		// Throw an exception if division by zero occurs
		throw std::invalid_argument("Division by zero in Vector2::operator/");
	}
	
}

Vector2& Vector2::operator/=(const float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2& vector) 
{
	os << "(" << vector.x << ", " << vector.y << ")";
	return os;
}
