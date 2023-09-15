// bullet.h

#ifndef BULLET_H
#define BULLET_H

typedef float Vec[3];

enum class OwnerType {
    PLAYER,
    ENEMY
};


class Bullet {
private:
    Vec pos;       // Bullet's position
    Vec vel;       // Bullet's velocity
    float radius;  // Bullet's size
    float angle;   // Direction of bullet's movement
    float speed;   // Speed of the bullet
    float lifespan; // Maximum distance bullet can travel
    float life_time; // Distance bullet has traveled so far
    OwnerType owner;

public:
    Bullet(float x, float y, float angle, OwnerType ownerType, double radius);
    bool collided;
    void update();
    void render() const;
    bool shouldRemove() const; // Check if bullet should be removed
    OwnerType getOwner() const;

    float getX() const;
    float getY() const;
    float getRadius() const;
    void setCollided(bool hit);

};

#endif
