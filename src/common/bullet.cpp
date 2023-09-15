// bullet.cpp

#include "bullet.h"
#include <cmath>
#include <GL/glx.h>
#include <cstdio>

#define PI 3.141592653589793

Bullet::Bullet(float x, float y, float angle, OwnerType ownerType, double radius)
: angle(angle), speed(5.0f), lifespan(100.0f), life_time(0.0f), owner(ownerType), radius(radius) {
    printf("Created Bullet in Bullet.cpp\n");
    fflush(stdout);
    pos[0] = x;
    pos[1] = y;
    pos[2] = 0.0f;
    //radius = 5.0f;

    // Initialize velocity based on the angle
    vel[0] = speed * cosf(angle);
    vel[1] = speed * sinf(angle);
    vel[2] = 0.0f;
    collided = false;
}

void Bullet::update() {
    // Update position based on velocity
    pos[0] += vel[0];
    pos[1] += vel[1];

    // Update distance traveled
    //distance_travelled += sqrt(vel[0]*vel[0] + vel[1]*vel[1]);
    //printf("Position Check: %f, %f",pos[0],pos[1]);
    life_time += 0.5f;

    
}

void Bullet::render() const {
    glColor3f(1.0f, 1.0f, 1.0f); // Green color for the sprite
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    int num_segments = 20;
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0 * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

bool Bullet::shouldRemove() const {
    if (life_time >= lifespan)
    {
        printf("lifetime exceeded");
        fflush(stdout);
    }
    return life_time >= lifespan || collided;
}

float Bullet::getX() const { return pos[0]; }
float Bullet::getY() const { return pos[1]; }
float Bullet::getRadius() const { return radius; }
void Bullet::setCollided(bool hit) { collided = hit; }
