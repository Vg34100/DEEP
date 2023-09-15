//Weapon.h
#include "bullet_manager.h"
#include <string>

class Weapon {
protected:
    double lastShotTime = 0.0;
    double shootCooldown; // Set this in constructor for different Weapons
    int damage;
    double radius;
    BulletManager& bulletManager;

public:
    std::string name;
    Weapon(BulletManager& bm) : bulletManager(bm) {}
    virtual ~Weapon() {}

    virtual void shoot(float x, float y, float angle);
};

class Pistol : public Weapon {
public:
    Pistol(BulletManager& bm) : Weapon(bm) {
        name = "Ol' Pistol";
        shootCooldown = 0.5; // example value
        damage = 2;
        radius = 15.0f;
    }


};


class MachineGun : public Weapon {
public:
    MachineGun(BulletManager& bm) : Weapon(bm) {
        name = "Random Machinery";
        shootCooldown = 0.05; // example value
        damage = 2;
        radius = 2.0f;
    }


};
// Add more child classes for other types of Weapons...
