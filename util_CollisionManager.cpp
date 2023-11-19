// CollisionManager.cpp
#include "util_CollisionManager.h"
#include "player.h"
const float TILE_SIZE = 80.0f;

int counterY = 0;
int counterX = 0;

bool current_level_completed = false;

void CollisionManager::handlePlayerCollisions(Player& player) {
	Vector2 playerPos = player.getPos();
	Vector2 potentialNewPosition = player.getPos();
	potentialNewPosition.add(player.getVelocity());

	int collisionResult = isCollidingWithWallsUsingRays(player.getPos(), potentialNewPosition);
	TileType currentTileType = static_cast<TileType>(collisionResult);
	if(world.AreThereNoEnemies()) {
		Level randomLevel = world.getNewLevel();
		float rowOffset = 0;
		float columnOffset = 0;
		current_level_completed = true;
		if (currentTileType == TileType::HALLWAYN) {
			//NORTH LEVEL
			counterY++;
			world.generateNewLevel(randomLevel, 0, world.getRows() - 2 - rowOffset);
			player.setPos(Vector2(playerPos.x,playerPos.y + 50));
			current_level_completed = false;
		}
		if (currentTileType == TileType::HALLWAYE) { // Eat
			//EAST LEVEL
			counterX++;
			world.generateNewLevel(randomLevel, world.getColumns() - 2 - columnOffset, 0);
			player.setPos(Vector2(playerPos.x + 50,playerPos.y));
			current_level_completed = false;
		}
		if (currentTileType == TileType::HALLWAYS) {
			//SOUTH LEVEL
			counterY--;
			world.generateNewLevel(randomLevel, 0, -world.getRows() + 2 + rowOffset);
			player.setPos(Vector2(playerPos.x ,playerPos.y - 50));
			current_level_completed = false;
		}
		if (currentTileType == TileType::HALLWAYW) { // Waffles
			//WEST LEVEL
			counterX--;
			world.generateNewLevel(randomLevel, -world.getColumns() + 2 + columnOffset, 0);
			player.setPos(Vector2(playerPos.x- 50 ,playerPos.y ));
			current_level_completed = false;
		}
	}



	// Check against walls with rays
	if (!collisionResult) {
		player.setPos(potentialNewPosition);
	} else {
		// Handle the sliding effect
		// Check in x-direction
		Vector2 xMove = player.getPos();
		xMove.add(Vector2(player.getVelocity().x, 0.0f));
		if (!isCollidingWithWallsUsingRays(player.getPos(), xMove) ) {
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
	// printf("Running Collision Check\n");
	// for (const auto& enemy : world.getEnemies()) {
	// 	enemy->moveToPlayer(player.getPos());
	// }
	if (auto weapon = player.getActiveWeapon()) {
		if (weapon->getIsAttacking()) {
			Hitbox weaponHitbox = weapon->getHitbox();
			for (const auto& enemy : world.getEnemies()) {
				enemy->moveToPlayer(player.getPos());
				if (weaponHitbox.isColliding(enemy->getHitbox())) {
					enemy->TakeDamage(weapon->getDamage());
				}
			}
		}
	}
	Hitbox playerHitbox = player.getHitbox();
	if(playerHitbox.topLeft.x) {
		for (const auto& enemy : world.getEnemies()) {
			if (playerHitbox.isCollidingExtra(enemy->getHitbox())) {
				player.TakeDamage(enemy->getDamage());
			}
		}
	}

}

std::string tileTypeToString(TileType type) {
	switch (type) {
		case TileType::WALL: return "WALL";
		case TileType::FLOOR: return "FLOOR";
		case TileType::HALLWAYN: return "HWN";
		case TileType::HALLWAYE: return "HWE";
		case TileType::HALLWAYS: return "HWS";
		case TileType::HALLWAYW: return "HWW";

		// ... other cases ...
		default: return "UNKNOWN";
	}
}


int CollisionManager::isCollidingWithWallsUsingRays(const Vector2& currentPos, const Vector2& newPos) const {
	// Define the ray endpoints
	Vector2 A = currentPos;
	Vector2 B = newPos;

	// Iterate through each tile in the world
	for (const auto& row : world.getGrid()) {
		for (const auto& tile : row) {
			if (tile.type == TileType::WALL || 
				( (tile.type == TileType::HALLWAYN || tile.type == TileType::HALLWAYS || 
				tile.type == TileType::HALLWAYW || tile.type == TileType::HALLWAYE) 
				&& !world.AreThereNoEnemies() ) ) {
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
					return  true; // Collision detected
				}
			}
			if (( (tile.type == TileType::HALLWAYN || tile.type == TileType::HALLWAYS || 
				tile.type == TileType::HALLWAYW || tile.type == TileType::HALLWAYE) 
				&& world.AreThereNoEnemies() ) ) {
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
					return static_cast<int>(tile.type); // Collision detected
				}
			}
		}
	}

	return  false;  // No collision detected
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

