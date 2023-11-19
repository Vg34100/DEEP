// CollisionManager.h
#pragma once
#include "world.h"
#include "util_vector2.h"

class Player;

extern bool current_level_completed;


class CollisionManager {
private:
	World& world; // Reference to the world to check collisions against

public:
	CollisionManager(World& w) : world(w) {};

	bool isCollidingWithTile(const Vector2& position, float halfWidth, float halfHeight) const;
	void handlePlayerCollisions(Player& player);
	void handleEnemyCollisions(Player& player);
	int isCollidingWithWallsUsingRays(const Vector2& currentPos, const Vector2& newPos) const;
	bool lineIntersectsLine(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D) const;
};
