#pragma once
#include <iostream>
struct Vector2 {
	float x, y;
	Vector2(float _x = 0, float _y = 0);
	void add(const Vector2& other);
	Vector2 multiply(float scalar);
	void normalize();
	float length() const;
	// New addition: Operator overloads
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;
	float dot(const Vector2& other) const;
	Vector2& operator+=(const Vector2& rhs);
};

struct Hitbox {
	Vector2 topLeft;
	Vector2 bottomRight;

	void print();

	bool contains(const Vector2& point) const;
	
	bool isColliding(const Hitbox& other) const;

	bool isCollidingExtra(const Hitbox& other) const;


};
