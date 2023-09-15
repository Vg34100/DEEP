// player.h
#ifndef PLAYER_H
#define PLAYER_H


#include "../Common/health.h"
#include "../Common/bullet_manager.h"
#include "../common/weapon.h"
#include <vector>

typedef float Vec[3];

class Player {
private:
    Vec pos;  // Player's position
    Vec vel;  // Player's velocity
    float color[3];  // Player's color
    float radius;
    float angle;
    Health playerHealth;
    Weapon* currentWeapon = nullptr;


public:
    Player(); // Modified constructor to accept BulletManager reference
    void handleInput();  // Handle user inputs to update velocity
    void update();  // Update player's position based on velocity
    void render();  // Render the player on screen
    void equipWeapon(Weapon* weapon);
    void shoot();
    float getX() const;
    float getY() const;
    void updateDirection(float mouseX, float mouseY);

};

#endif
