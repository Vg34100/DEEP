// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "../setup/global.h"
#include "../Common/health.h"
#include <string>
typedef float Vec[3];


class Enemy {
protected:
    Vec pos;
    Vec vel;
    float color[3];
    float size;
    std::string name;
    int damage;
    Health health;
    void moveTowards(float x, float y);  // Basic AI to move towards a point
    bool isDead;
public:
    Enemy();
    void update(float playerX, float playerY);  // The AI logic will be here
    void render();
    void onCollision();  // Called when the enemy collides with a bullet
    float getX() const;
    float getY() const;
    float getSize() const;
    bool IsDead() const;
};

class BasicEnemy : public Enemy {
public:
    BasicEnemy();
};

// You can add more enemy types as needed...

#endif