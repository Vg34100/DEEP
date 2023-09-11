#include "player.h"
#include "../setup/global.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cmath>

#define PI 3.141592653589793

Player::Player() {
    // Initialize the player's properties
    pos[0] = gl.xres / 2.0f;
    pos[1] = gl.yres / 2.0f;
    pos[2] = 0.0f;
    
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 0.0f;

    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;

    radius = 20.0f;
}

void Player::handleInput() {
    vel[0] = 0.0f;
    vel[1] = 0.0f;

    const float speed = 4.0f;

    if (gl.keys[XK_Left] || gl.keys[XK_a]) vel[0] = -speed;
    if (gl.keys[XK_Right] || gl.keys[XK_d]) vel[0] = speed;
    if (gl.keys[XK_Up] || gl.keys[XK_w]) vel[1] = speed;
    if (gl.keys[XK_Down] || gl.keys[XK_s]) vel[1] = -speed;
}


void Player::update() {
    pos[0] += vel[0];
    pos[1] += vel[1];

    // Boundary checking to ensure player stays within screen
    if (pos[0] < 0) pos[0] = 0;
    if (pos[0] > gl.xres) pos[0] = gl.xres;
    if (pos[1] < 0) pos[1] = 0;
    if (pos[1] > gl.yres) pos[1] = gl.yres;
}

void Player::render() {
    glColor3fv(color);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    int num_segments = 50;
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0 * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();

    // Draw sprite in front of player
    float spriteDistance = 30.0; // Change as needed
    float spriteX = pos[0] + spriteDistance * cos(angle);
    float spriteY = pos[1] + spriteDistance * sin(angle);
    
    // Set sprite color and size
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for the sprite
    float size = 10.0; // Size of the sprite

    // Render the sprite as a box
    glBegin(GL_QUADS);
    glVertex2f(spriteX - size, spriteY - size);
    glVertex2f(spriteX + size, spriteY - size);
    glVertex2f(spriteX + size, spriteY + size);
    glVertex2f(spriteX - size, spriteY + size);
    glEnd();


}


void Player::updateDirection(float mouseX, float mouseY) {
    angle = atan2(mouseY - pos[1], mouseX - pos[0]);
}