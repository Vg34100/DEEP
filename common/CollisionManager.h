// CollisionManager.h
#pragma once
#include "../world/world.h"
#include "vector2.h"

class Player;

class CollisionManager {
private:
    World& world; // Reference to the world to check collisions against

public:
    CollisionManager(World& w) : world(w) {};

    bool isCollidingWithTile(const Vector2& position, float halfWidth, float halfHeight) const;
    void handlePlayerCollisions(Player& player);
    void handleEnemyCollisions(Player& player);
    bool isCollidingWithWallsUsingRays(const Vector2& currentPos, const Vector2& newPos) const;
    bool lineIntersectsLine(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D) const;
    TileType getPlayerCurrentTileType(const Player& player);

};