// bullet_manager.h

#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <vector>
#include "bullet.h"

class BulletManager {
private:
    std::vector<Bullet> bullets;

public:
    void addBullet(float x, float y, float angle, OwnerType ownerType, double radius); // Add a new bullet
    void updateBullets(); // Update all bullets
    void renderBullets(); // Render all bullets
    void cleanupBullets(); // Remove bullets that should be deleted
    std::vector<Bullet>& getBullets();
};

#endif
