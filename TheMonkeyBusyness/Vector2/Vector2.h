#pragma once
#include <iostream>
#include <concepts>


#ifdef VECTOR2_EXPORTS
#define VECTOR2_API __declspec(dllexport)  // Export template class and functions
#else
#define VECTOR2_API __declspec(dllimport)
#endif

template <std::floating_point T>
class VECTOR2_API Vector2 {
	//static_assert(std::is_floating_point_v<T>, "Vector2 can only be instantiated with floating-point types (float or double).");
public:
	T x;
	T y;
	Vector2(T x = 0, T y = 0);
	static Vector2 Normalize(const Vector2 vector2);
	void Normalize();
	Vector2 operator*(const Vector2 &vector)const;
	Vector2 operator*(const T scalar)const;
	Vector2& operator*=(const T scalar);
	Vector2 operator/(const T scalar)const;
	Vector2& operator/=(const T scalar);
	Vector2 operator+(const Vector2& vector) const;
	Vector2& operator+=(const Vector2& vector);
	Vector2 operator-(const Vector2& other) const;
	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Vector2& vector); 
	T GetAngleFromNormalizedVector() const;
};

