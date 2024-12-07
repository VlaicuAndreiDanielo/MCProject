#pragma once
#include <iostream>
class Vector2 {
public:
	float x;
	float y;
	Vector2(float x=0, float y=0);
	static Vector2 Normalize(const Vector2 vector2);
	void Normalize();
	Vector2 operator*(const Vector2 &vector)const;
	Vector2 operator*(const float scalar)const;
	Vector2& operator*=(const float scalar);
	Vector2 operator/(const float scalar)const;
	Vector2& operator/=(const float scalar);
	Vector2 operator+(const Vector2& vector) const;
	Vector2& operator+=(const Vector2& vector);
	Vector2 operator-(const Vector2& other) const;
	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Vector2& vector); 
	float GetAngleFromNormalizedVector() const;
};