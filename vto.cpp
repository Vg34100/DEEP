//Author: Vincent To
//Weapons
#include "vto.h"
#include "player.h"
#include <ctime>
#include "prodriguezqu.h"
#include "image.h"

Vector2 rotatePoint(const Vector2& point, float angle, const Vector2& center) {
    float s = sin(angle);
    float c = cos(angle);

    // Translate point back to the origin
    Vector2 p = point - center;

    // Rotate point
    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;

    // Translate point back to original position
    p.x = xnew + center.x;
    p.y = ynew + center.y;
    return p;
}

void Weapon::showHitbox() const {
    // Draw the rectangle outline
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0f);  // Color it red for visibility
    glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
    glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
    glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
    glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
    glEnd();
}

Lightsaber::Lightsaber(Player *p) : player(p) {
    name = "Lithium Ion Grip Heat Transfer-Saber";
    damage = 50; 
    cooldown = 0.05;
    attackSize = 1;
    speed = 0;
    duration = 0.05;
    damageType = "default";
    weaponClass = "Melee";
    if (!idle.loadTexture()) {
        std::cerr << "Failed to load texture" << std::endl;
    }
    idle.setSpriteSheet(1,4);
}

void Lightsaber::use() {
    if (!onCooldown && !isAttacking) {
        isAttacking = true;
        lastUseTime = std::clock();  // Record the current time
    }
}

void Lightsaber::update() {
    if (isAttacking) {
        float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;

        // Check if the attack duration has passed
        if (elapsedTime >= duration) {
            isAttacking = false;
            onCooldown = true;
            lastUseTime = std::clock(); // Reset the timer for cooldown
        }
    } else if (onCooldown) {
        float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;

        // Check if the cooldown has passed
        if (elapsedTime >= cooldown) {
            onCooldown = false;
        }
    }
}
void Lightsaber::render() {
    update();
    Vector2 direction = player->getDirection();
    Vector2 position = player->getPos();

    float distanceFromPlayer = 120.0f;  
    Vector2 startPos = position + direction.multiply(distanceFromPlayer);
    float indicatorWidth = 250.0f * attackSize;
    float indicatorHeight = 200.0f * attackSize; 
    if (!isAttacking) {
        idle.renderSprite(0, 1, startPos.x, startPos.y, 100);
    }
    if (isAttacking) {
        idle.renderSprite(0, 3, startPos.x, startPos.y, 100);
    }

    Vector2 topLeft = Vector2(startPos.x - 0.2f * indicatorWidth, 
            startPos.y - 0.4f * indicatorHeight);
    Vector2 bottomRight = Vector2(startPos.x + 0.2f * indicatorWidth, 
            startPos.y + 0.4f * indicatorHeight);

    hitbox.topLeft = topLeft;
    hitbox.bottomRight = bottomRight;
#ifdef DEBUG
    showHitbox();
#endif
}

Gun::Gun(Player *p) : player(p) {
    name = "Gun";
    damage = 50; 
    cooldown = 0.025;
    attackSize = 1;
    speed = 5;
    duration = 1;
    damageType = "default";
    weaponClass = "Range";
}

void Gun::use() {
    if (!onCooldown && !isAttacking) {
        isAttacking = true;
        lastUseTime = std::clock();  // Record the current time
    }
}

void Gun::update() {
    if (isAttacking) {
        float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;

        // Check if the attack duration has passed
        if (elapsedTime >= duration) {
            isAttacking = false;
            onCooldown = true;
            lastUseTime = std::clock(); // Reset the timer for cooldown
        }
    } else if (onCooldown) {
        float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;

        // Check if the cooldown has passed
        if (elapsedTime >= cooldown) {
            onCooldown = false;
        }
    }
}
void Gun::render() {
    update();
    Vector2 direction = player->getDirection();
    Vector2 position = player->getPos();

    float distanceFromPlayer = 120.0f;  
    Vector2 startPos = position + direction.multiply(distanceFromPlayer);
    float indicatorWidth = 250.0f;
    float indicatorHeight = 200.0f; 

    if (isAttacking) {
        // Draw the rectangle
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(startPos.x - 0.5f * indicatorWidth, startPos.y - 0.5f *
                indicatorHeight);
        glVertex2f(startPos.x + 0.5f * indicatorWidth, startPos.y - 0.5f * 
                indicatorHeight);
        glVertex2f(startPos.x + 0.5f * indicatorWidth, startPos.y + 0.5f * 
                indicatorHeight);
        glVertex2f(startPos.x - 0.5f * indicatorWidth, startPos.y + 0.5f * 
                indicatorHeight);
        glEnd();
    }
    Vector2 topLeft = Vector2(startPos.x - 0.5f * indicatorWidth, 
            startPos.y - 0.5f * indicatorHeight);
    Vector2 bottomRight = Vector2(startPos.x + 0.5f * indicatorWidth, 
            startPos.y + 0.5f * indicatorHeight);

    hitbox.topLeft = topLeft;
    hitbox.bottomRight = bottomRight;
#ifdef DEBUG
    showHitbox();
#endif
}


//Statistics for game
int renderFunctionCalls(const bool renderFun) 
{
    static int actualCount = 1;
    if (renderFun) {
        actualCount++;
        renderRect("Redner has been called: ", actualCount);
    } else {
        return 0;
    }
    return 0 ;
}
