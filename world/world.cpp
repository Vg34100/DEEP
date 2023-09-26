//world/world.cpp
#include "world.h"
#include <ctime>
// Colors for rendering
const float FLOOR_COLOR[] = {0.5f, 0.5f, 0.5f};
const float DOOR_COLOR[] = {0.6f, 0.6f, 0.6f};
const float WALL_COLOR[] = {0.1f, 0.1f, 0.1f};
const float PIT_COLOR[] = {0.0f, 0.0f, 0.0f};
const float ENEMY_COLOR[] = {0.4f, 0.4f, 0.4f};
const float TILE_SIZE = 128.0f;
const float HALLWAY_COLOR[] = {0.7f, 0.7f, 0.7f};

World::World() {
    LevelManager manager;
    // 1. Get a random initial level and set the world's offset to center it.
    Level initialLevel = manager.getRandomLevel(); // Assuming you have a getRandomLevel method in LevelManager
    initialized = initialLevel;
    this->setWorldFromLevel(initialized); 
}

Level World::getNewLevel() {
    LevelManager manager;
    Level newLevel = manager.getRandomLevel();
    return newLevel;
}

void World::generateNewLevel(const Level& level, float customOffsetX, float customOffsetY) {
    //Level sample = Level::SampleLevel();
    this->setWorldFromLevel(level, customOffsetX, customOffsetY); 
    initialized = level;
    WorldCorners corners = getWorldDimensions();
    std::cout << "Top Left: (" << corners.topLeft.x << ", " << corners.topLeft.y << ")\n";
    std::cout << "Top Right: (" << corners.topRight.x << ", " << corners.topRight.y << ")\n";
    std::cout << "Bottom Left: (" << corners.bottomLeft.x << ", " << corners.bottomLeft.y << ")\n";
    std::cout << "Bottom Right: (" << corners.bottomRight.x << ", " << corners.bottomRight.y << ")\n";
}



WorldCorners World::calculateLevelDimensions(const Level& level, float customOffsetX, float customOffsetY) const {
    WorldCorners corners;

    // Extract the layout from the level
    const std::vector<std::string>& layout = level.layout();

    // Calculate potential offsets
    float potentialOffsetX = customOffsetX;
    float potentialOffsetY = customOffsetY;
    
    if (potentialOffsetX == 0.0f) {
        potentialOffsetX = (layout[0].size() * TILE_SIZE) / 2.0f;
    }
    
    if (potentialOffsetY == 0.0f) {
        potentialOffsetY = (layout.size() * TILE_SIZE) / 2.0f;
    }

    // Calculate world width and height based on the layout
    float worldWidth = layout[0].size() * TILE_SIZE; // Assuming a uniform grid
    float worldHeight = layout.size() * TILE_SIZE;

    // Calculate the corners based on the potential offsets
    corners.topLeft = Vector2(-potentialOffsetX, worldHeight - potentialOffsetY);
    corners.topRight = Vector2(worldWidth - potentialOffsetX, worldHeight - potentialOffsetY);
    corners.bottomLeft = Vector2(-potentialOffsetX, -potentialOffsetY);
    corners.bottomRight = Vector2(worldWidth - potentialOffsetX, -potentialOffsetY);

    return corners;
}


float World::getTopEdgeY() const {
    // Calculate world height
    float worldHeight = grid.size() * TILE_SIZE;
    return worldHeight - offsetY;  // Top edge Y position in world space
}


void World::setWorldFromLevel(const Level& level, float customOffsetX, float customOffsetY) {
    offsetX += customOffsetX;
    offsetY += customOffsetY;
    customOffsetX = offsetX;
    customOffsetY = offsetY;
    customOffsetY = -customOffsetY;
    grid.clear();
    const std::vector<std::string>& layout = level.layout();
    const std::vector<EnemyType>& enemyTypes = level.enemyTypes();

    // Half-width and half-height (in tiles) of the layout
    float halfWidth = layout[0].size() / 2.0f;
    float halfHeight = layout.size() / 2.0f;

    size_t enemyIndex = 0; // to keep track of the enemy types

    for (size_t y = 0; y < layout.size(); y++) {
        std::vector<Tile> row;
        for (size_t x = 0; x < layout[y].size(); x++) {
            Tile tile; 
            
            tile.x = x - halfWidth + customOffsetX;
            tile.y = halfHeight - y - customOffsetY;


            switch (layout[y][x]) {
                case 'x':
                    tile.type = TileType::FLOOR;
                    break;
                case '-':
                    tile.type = TileType::WALL;
                    break;
                case '^':
                    tile.type = TileType::PIT;
                    break;
                case '|':
                    tile.type = TileType::DOOR;
                    break; 
                case 'N':
                    tile.type = TileType::HALLWAYN;
                    break;
                case 'E':
                    tile.type = TileType::HALLWAYE;
                    break;
                case 'S':
                    tile.type = TileType::HALLWAYS;
                    break;
                case 'W':
                    tile.type = TileType::HALLWAYW;
                    break;
                case '*':
                    tile.type = TileType::ENEMY;
                    if (enemyIndex < enemyTypes.size()) {
                        tile.enemy = enemyTypes[enemyIndex];
                        spawnEnemy(tile.enemy, Vector2(tile.x * TILE_SIZE - offsetX, tile.y * TILE_SIZE - offsetY));
                        enemyIndex++;
                    }
                    break;
            }
            row.push_back(tile);
        }
        grid.push_back(row);
    }
}


World::~World() {
}

void World::spawnEnemy(EnemyType type, Vector2 pos) {
    printf("Spawning enemy of type: %d at position X: %f, Y: %f\n", static_cast<int>(type), pos.x, pos.y);
    fflush(stdout);
    switch (type) {
        case EnemyType::ORC:
            enemies.push_back(std::make_unique<Orc>(*this, pos));
            break;
        case EnemyType::NONE:
            printf("NONE");
            fflush(stdout);
            break;
        // ... handle other enemy types
    }
}


void World::render() {
    this->RemoveDeadEnemies();
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            switch (tile.type) {
                case TileType::FLOOR:
                    glColor3fv(FLOOR_COLOR);
                    break;
                case TileType::WALL:
                    glColor3fv(WALL_COLOR);
                    break;
                case TileType::PIT:
                    glColor3fv(PIT_COLOR);
                    break;
                case TileType::DOOR:
                    glColor3fv(DOOR_COLOR);
                    break;
                case TileType::ENEMY:
                    glColor3fv(ENEMY_COLOR);
                    break;
                case TileType::HALLWAYN:
                    glColor3fv(HALLWAY_COLOR);
                    break;
                case TileType::HALLWAYE:
                    glColor3fv(HALLWAY_COLOR);
                    break;
                case TileType::HALLWAYS:
                    glColor3fv(HALLWAY_COLOR);
                    break;
                case TileType::HALLWAYW:
                    glColor3fv(HALLWAY_COLOR);
                    break;
            }

            glBegin(GL_QUADS);
            glVertex2f(tile.x * TILE_SIZE - offsetX, tile.y * TILE_SIZE - offsetY);
            glVertex2f((tile.x + 1) * TILE_SIZE - offsetX, tile.y * TILE_SIZE - offsetY);
            glVertex2f((tile.x + 1) * TILE_SIZE - offsetX, (tile.y + 1) * TILE_SIZE - offsetY);
            glVertex2f(tile.x * TILE_SIZE - offsetX, (tile.y + 1) * TILE_SIZE - offsetY);
            glEnd();
        }
    }
}

void World::renderEnemies() const {
    for (const auto& enemy : enemies) {
        enemy->render();
    }
}


TileType World::getTileTypeAt(int x, int y) const {
    return grid[y][x].type;
}

bool World::isWalkable(int x, int y) const {
    TileType type = getTileTypeAt(x, y);
    return type == TileType::FLOOR || type == TileType::ENEMY || type == TileType::DOOR;
}

TileType World::getTileTypeAtPosition(float x, float y) {
    int totalRows = grid.size();
    int totalCols = (totalRows > 0) ? grid[0].size() : 0; // Assuming the grid is rectangular

    // Half-width and half-height (in tiles) of the grid
    float halfWidth = totalCols / 2.0f;
    float halfHeight = totalRows / 2.0f;

    // Account for offsets
    float adjustedX = x - offsetX * TILE_SIZE;
    float adjustedY = y - offsetY * TILE_SIZE;

    // Calculate grid indices from the adjusted position
    size_t i = static_cast<size_t>(halfHeight - (adjustedY / TILE_SIZE));
    size_t j = static_cast<size_t>(adjustedX / TILE_SIZE + halfWidth);

    // Bounds check
    if (i >= 0 && i < static_cast<size_t>(totalRows) && j >= 0 && j < grid[i].size()) {
        return grid[i][j].type;
    }

    return TileType::WALL;  // Default to wall if out of bounds or undefined
}



bool World::isWalkableAtPosition(float x, float y) const {
    size_t i = static_cast<size_t>(y / (TILE_SIZE)); 
    size_t j = static_cast<size_t>(x / (TILE_SIZE)); 


    // Bounds check
    if (i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size()) {
        return isWalkable(j, i);
    }
    return false;  // Default to non-walkable if out of bounds
}


const std::vector<std::vector<Tile>>& World::getGrid() const {
    return grid;
}

float World::getOffsetX() const {
    return offsetX;
}

float World::getOffsetY() const {
    return offsetY;
}

WorldCorners World::getWorldDimensions() const {
    WorldCorners corners;

    // Calculate world width and height
    float worldWidth = grid[0].size() * TILE_SIZE; // Assuming a uniform grid
    float worldHeight = grid.size() * TILE_SIZE;

    // Calculate the corners
    corners.topLeft = Vector2(-offsetX, worldHeight - offsetY);
    corners.topRight = Vector2(worldWidth - offsetX, worldHeight - offsetY);
    corners.bottomLeft = Vector2(-offsetX, -offsetY);
    corners.bottomRight = Vector2(worldWidth - offsetX, -offsetY);

    return corners;
}

void World::RemoveDeadEnemies() {
    float currentTime = std::clock();
    float delay = 0.0f * CLOCKS_PER_SEC;  // 1 second delay

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [currentTime, delay](const std::unique_ptr<Enemy>& enemy) {
            if (!enemy) {
                std::cerr << "Null pointer detected in enemies list." << std::endl;
                return true; // remove the null pointer from the list
            }

            // Check if the enemy is marked dead and if enough time has passed
            if (enemy->isDeadCheck() && (currentTime - enemy->getDeathTimestamp()) > delay) {
                return true;
            }

            return false;
        }), enemies.end());
}



bool World::AreThereNoEnemies() const {
    return enemies.empty();
}