#include "weapon.h"
#include <chrono>

void Weapon::shoot(float x, float y, float angle) {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;

    if ((currentTime - lastShotTime) >= shootCooldown) {
        bulletManager.addBullet(x, y, angle, OwnerType::PLAYER, radius);
        lastShotTime = currentTime;
    }
}
