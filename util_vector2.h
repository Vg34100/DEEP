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

	void print() {
		std::cout << topLeft.x << ", " << topLeft.y << std::endl;
		std::cout << bottomRight.x << ", " << bottomRight.y << std::endl;

	}

	bool contains(const Vector2& point) const {
		return point.x >= topLeft.x && point.x <= bottomRight.x &&
			   point.y >= topLeft.y && point.y <= bottomRight.y;
	}
	
	bool isColliding(const Hitbox& other) const {
		std::cout << topLeft.x << ", " << topLeft.y << std::endl;
		std::cout << bottomRight.x << ", " << bottomRight.y << std::endl;
		std::cout << other.topLeft.x << ", " << other.topLeft.y << std::endl;
		std::cout << other.bottomRight.x << ", " << other.bottomRight.y << std::endl;
		// std::cout << "CHECKING COLLISION" << std::endl;
		// Check if any corner of the other hitbox is inside this hitbox
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
		std::cout << "FALSE" << std::endl;
		return false;
	}

	bool isColliding2(const Hitbox& other) const {
		return !(topLeft.x > other.bottomRight.x ||
				bottomRight.x < other.topLeft.x ||
				topLeft.y < other.bottomRight.y ||
				bottomRight.y > other.topLeft.y);
}


};
