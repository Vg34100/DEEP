//world.h
#pragma once

#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <memory>
#include "enemy.h"
#include "world_level.h"
#include "util_enemytype.h"

extern int levelsCompleted;
extern int enemiesRemaining;
extern int enemiesDefeated;
class Enemy;

struct WorldCorners {
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;
};

// Enum to represent different tiles in the world
enum class TileType {
	FLOOR, 
	WALL,
	PIT,
	ENEMY,
	DOOR,
	HALLWAYN,
	HALLWAYE,
	HALLWAYS,
	HALLWAYW
};


// Represent each tile
struct Tile {
	TileType type;
	EnemyType enemy = EnemyType::NONE; // Default to NONE
	float x, y; // Position of tile
};

// The level/world class
class World {
private:
	std::vector<std::vector<Tile>> grid;
	float offsetX, offsetY;
	std::vector<std::unique_ptr<Enemy>> enemies;
	Level initialized = Level::SampleLevel();

public:
	World();
	~World();
	void render(); // Render the tiles using OpenGL

	// Utility methods
	TileType getTileTypeAt(int x, int y) const;
	bool isWalkable(int x, int y) const;
	TileType getTileTypeAtPosition(float x, float y);
	bool isWalkableAtPosition(float x, float y) const;
	void spawnEnemy(EnemyType type, Vector2 pos);
	void renderEnemies() const;
	int countEnemies() const;
	const std::vector<std::vector<Tile>>& getGrid() const;
	float getOffsetX() const;
	float getOffsetY() const;
	const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
	void setWorldFromLevel(const Level& level, float customOffsetX = 0.0f, float customOffsetY = 0.0f);
	void generateNewLevel(const Level& level, float customOffsetX = 0.0f, float customOffsetY = 0.0f);
	WorldCorners getWorldDimensions() const;
	WorldCorners calculateLevelDimensions(const Level& level, float customOffsetX = 0.0f, float customOffsetY = 0.0f) const;
	Level getNewLevel();

	void resetWorld();

	float getTopEdgeY() const;
	void RemoveDeadEnemies();
	bool AreThereNoEnemies() const;
	int getRows() const {
		return initialized.getRows();
	}

	int getColumns() const {
		return initialized.getColumns();
	}
};

