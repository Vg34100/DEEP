// enemy.cpp
#include "enemy.h"
#include <GL/glx.h>
#include <string>
#include <cmath>
#include "../setup/global.h"
#include "../setup/fonts.h"


Enemy::Enemy() : health(100), isDead(false) {
    // default values
    pos[0] = gl.xres * 0.1;  // For testing, spawn at the left side of the screen
    pos[1] = gl.yres * 0.5;
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    color[0] = 1.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    size = 20.0f;
    //health = Health(100);  // default health value
}

void Enemy::moveTowards(float x, float y) {
    // Basic logic to move towards a point
    float speed = 1.0f;
    float angle = atan2(y - pos[1], x - pos[0]);
    vel[0] = speed * cosf(angle);
    vel[1] = speed * sinf(angle);
}

void Enemy::update(float playerX, float playerY) {
    moveTowards(playerX, playerY);
    pos[0] += vel[0];
    pos[1] += vel[1];
}

void Enemy::render() {
    glColor3fv(color);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-size/2, -size/2);
    glVertex2f(size/2, -size/2);
    glVertex2f(size/2, size/2);
    glVertex2f(-size/2, size/2);
    glEnd();
    glPopMatrix();

    // Health GUI
    std::string healthText = "Health: " + std::to_string(health.GetCurrentHealth()) + "/" + std::to_string(health.GetMaxHealth());
	Rect r;
	//glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = pos[1] + size/2;
	r.left = pos[0];
	r.center = pos[0];
    ggprint8b(&r, 16, 0x00ff0000, healthText.c_str());
    glEnd();
}

void Enemy::onCollision() {
    health.TakeDamage(10);  // decrease by bullet's damage
    if (health.GetCurrentHealth() <= 0) {
        // handle enemy death, like removing from game or spawning loot
        isDead = true;
        // Remove dead enemies
    }
    
}

float Enemy::getX() const { return pos[0]; }
float Enemy::getY() const { return pos[1]; }
float Enemy::getSize() const { return size; }
bool Enemy::IsDead() const { return isDead; }


BasicEnemy::BasicEnemy() : Enemy() {
    // Setup specific values for BasicEnemy
    name = "BasicEnemy";
    damage = 1;
    color[0] = 1.0f;
    color[1] = 0.5f;
    color[2] = 0.5f;
    size = 30.0f;
}