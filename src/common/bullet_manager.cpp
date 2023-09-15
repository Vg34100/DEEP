// bullet_manager.cpp

#include "bullet_manager.h"
#include <algorithm> // For std::remove_if
#include <cstdio>

void BulletManager::addBullet(float x, float y, float angle, OwnerType ownerType, double radius) {
    Bullet newBullet(x, y, angle, ownerType, radius);
    bullets.push_back(newBullet);
    //printf("BulletManager\n");
    fflush(stdout);
}

void BulletManager::updateBullets() {
    for (Bullet &bullet : bullets) {
        bullet.update();
    }
}

void BulletManager::renderBullets() {
    for (const Bullet &bullet : bullets) {
        bullet.render();
    }
}

void BulletManager::cleanupBullets() {
    //printf("Remove");
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet &bullet) {
            return bullet.shouldRemove();
        }), bullets.end());
}

std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}
