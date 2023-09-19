// CollisionManager.cpp
#include "CollisionManager.h"
#include "../player/player.h"
const float TILE_SIZE = 128.0f;

int counterY = 0;
int counterX = 0;
bool CollisionManager::isCollidingWithTile(const Vector2& position, float halfWidth, float halfHeight) const {
    // Check four corners of the bounding box
    if (world.getTileTypeAtPosition(position.x - halfWidth, position.y - halfHeight) != TileType::FLOOR) return true;
    if (world.getTileTypeAtPosition(position.x + halfWidth, position.y - halfHeight) != TileType::FLOOR) return true;
    if (world.getTileTypeAtPosition(position.x - halfWidth, position.y + halfHeight) != TileType::FLOOR) return true;
    if (world.getTileTypeAtPosition(position.x + halfWidth, position.y + halfHeight) != TileType::FLOOR) return true;

    return false;
}

TileType CollisionManager::getPlayerCurrentTileType(const Player& player) {
    // Get player's current position

    Vector2 playerPos = player.getPos();
    
    // Fetch the tile type at that position
    TileType currentTileType = world.getTileTypeAtPosition(playerPos.x, playerPos.y);
    // WorldCorners corners = world.getWorldDimensions();
    // WorldCorners newcorners = world.calculateLevelDimensions(randomLevel);
    // Print the result
    // std::cout << "Player is currently on tile: " << static_cast<int>(currentTileType) << std::endl;
    if(!world.AreThereNoEnemies())
        return currentTileType;;
    Level randomLevel = world.getNewLevel();
    float rowOffset;
    float columnOffset;
    if (randomLevel.getRows() > world.getRows())
        rowOffset = -abs(world.getRows() - randomLevel.getRows())/2;
    else
        rowOffset = 0;

    if (randomLevel.getColumns() > world.getColumns())
        rowOffset = abs(world.getColumns() - randomLevel.getColumns())/2;
    else
        rowOffset = -abs(world.getColumns() - randomLevel.getColumns())/2;

    if (currentTileType == TileType::HALLWAYN) {
        //NORTH LEVEL
        counterY++;
        world.generateNewLevel(randomLevel, 0, world.getRows() - 1 - rowOffset);
    }
    if (currentTileType == TileType::HALLWAYE) { // Eat
        //EAST LEVEL
        counterX++;
        world.generateNewLevel(randomLevel, world.getColumns() -1 - columnOffset, 0);
    }
    if (currentTileType == TileType::HALLWAYS) {
        //SOUTH LEVEL
        counterY--;
        world.generateNewLevel(randomLevel, 0, -world.getRows() + 1 + rowOffset);
    }
    if (currentTileType == TileType::HALLWAYW) { // Waffles
        //WEST LEVEL
        counterX--;
        world.generateNewLevel(randomLevel, -world.getColumns() + 1 + columnOffset, 0);
    }
    return currentTileType;
}


void CollisionManager::handlePlayerCollisions(Player& player) {
    getPlayerCurrentTileType(player);
    Vector2 potentialNewPosition = player.getPos();
    potentialNewPosition.add(player.getVelocity());

    // Check against walls with rays
    if (!isCollidingWithWallsUsingRays(player.getPos(), potentialNewPosition)) {
        player.setPos(potentialNewPosition);
    } else {
        // Handle the sliding effect
        // Check in x-direction
        Vector2 xMove = player.getPos();
        xMove.add(Vector2(player.getVelocity().x, 0.0f));
        if (!isCollidingWithWallsUsingRays(player.getPos(), xMove)) {
            player.setX(xMove.x);
        }

        // Check in y-direction
        Vector2 yMove = player.getPos();
        yMove.add(Vector2(0.0f, player.getVelocity().y));
        if (!isCollidingWithWallsUsingRays(player.getPos(), yMove)) {
            player.setY(yMove.y);
        }
    }
}

void CollisionManager::handleEnemyCollisions(Player& player) {
    if (auto weapon = player.getActiveWeapon()) {
        if (weapon->getIsAttacking()) {
            Hitbox weaponHitbox = weapon->getHitbox();
            for (const auto& enemy : world.getEnemies()) {
                if (weaponHitbox.isColliding(enemy->getHitbox())) {
                    enemy->TakeDamage(weapon->getDamage());
                }
            }
        }
    }
}



bool CollisionManager::isCollidingWithWallsUsingRays(const Vector2& currentPos, const Vector2& newPos) const {
    // Define the ray endpoints
    Vector2 A = currentPos;
    Vector2 B = newPos;

    // Iterate through each tile in the world
    for (const auto& row : world.getGrid()) {
        for (const auto& tile : row) {
            if (tile.type == TileType::WALL) {
                // Define the 4 corners of the tile
                Vector2 topLeft(tile.x * TILE_SIZE - world.getOffsetX(), tile.y * TILE_SIZE - world.getOffsetY());
                Vector2 topRight((tile.x + 1) * TILE_SIZE - world.getOffsetX(), tile.y * TILE_SIZE - world.getOffsetY());
                Vector2 bottomLeft(tile.x * TILE_SIZE - world.getOffsetX(), (tile.y + 1) * TILE_SIZE - world.getOffsetY());
                Vector2 bottomRight((tile.x + 1) * TILE_SIZE - world.getOffsetX(), (tile.y + 1) * TILE_SIZE - world.getOffsetY());

                // Check intersection with each side of the tile
                if (lineIntersectsLine(A, B, topLeft, topRight) ||
                    lineIntersectsLine(A, B, topRight, bottomRight) ||
                    lineIntersectsLine(A, B, bottomRight, bottomLeft) ||
                    lineIntersectsLine(A, B, bottomLeft, topLeft)) {
                    return true; // Collision detected
                }
            }
        }
    }

    return false;  // No collision detected
}

bool CollisionManager::lineIntersectsLine(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D) const {
    float detACD = (A.x - C.x) * (D.y - C.y) - (D.x - C.x) * (A.y - C.y);
    float detBCD = (B.x - C.x) * (D.y - C.y) - (D.x - C.x) * (B.y - C.y);
    float detCAB = (C.x - A.x) * (B.y - A.y) - (B.x - A.x) * (C.y - A.y);
    float detDAB = (D.x - A.x) * (B.y - A.y) - (B.x - A.x) * (D.y - A.y);

    // If determinants have different signs, they straddle each other
    if ((detACD * detBCD) < 0 && (detCAB * detDAB) < 0) {
        return true;
    }
    return false;
}