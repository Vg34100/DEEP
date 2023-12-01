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

void Hitbox::print() 
{
	std::cout << topLeft.x << ", " << topLeft.y << std::endl;
	std::cout << bottomRight.x << ", " << bottomRight.y << std::endl;

}

bool Hitbox::contains(const Vector2& point) const 
{
	return point.x >= topLeft.x && point.x <= bottomRight.x &&
			point.y >= topLeft.y && point.y <= bottomRight.y;
}

bool Hitbox::isColliding(const Hitbox& other) const 
{
	if (contains(other.topLeft) ||
		contains({other.bottomRight.x, other.topLeft.y}) ||
		contains({other.topLeft.x, other.bottomRight.y}) ||
		contains(other.bottomRight)) {
		return true;
	}

	// Check if any corner of this hitbox is inside the other hitbox
	if (other.contains(topLeft) ||
		other.contains({bottomRight.x, topLeft.y}) ||
		other.contains({topLeft.x, bottomRight.y}) ||
		other.contains(bottomRight)) {
		return true;
	}
	// std::cout << "FALSE" << std::endl;
	return false;
}

bool Hitbox::isCollidingExtra(const Hitbox& other) const 
{
	return !(topLeft.x > other.bottomRight.x ||
			bottomRight.x < other.topLeft.x ||
			topLeft.y < other.bottomRight.y ||
			bottomRight.y > other.topLeft.y);
}
