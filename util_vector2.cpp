#include "util_vector2.h"
#include <cmath>
Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

void Vector2::add(const Vector2& other) 
{
	x += other.x;
	y += other.y;
}

Vector2 Vector2::multiply(float scalar) 
{
	return Vector2(x * scalar, y * scalar);
}

void Vector2::normalize() 
{
	float length = sqrt(x * x + y * y);
	if (length > 0.0f) {
		x /= length;
		y /= length;
	}
}

float Vector2::length() const 
{
	return sqrt(x*x + y*y);
}

float Vector2::dot(const Vector2& other) const 
{
	return x * other.x + y * other.y;
}

Vector2 Vector2::operator-(const Vector2& other) const 
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator+(const Vector2& other) const 
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator/(float scalar) const 
{
	return Vector2(x / scalar, y / scalar);
}

Vector2 Vector2::operator*(float scalar) const 
{
	return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator+=(const Vector2& rhs) 
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

