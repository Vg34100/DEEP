//player.cpp
#include "player.h"
#include "../setup/global.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cmath>
#include "../Common/health.h"
#include <string>
#include "../setup/fonts.h"

#define PI 3.141592653589793



Player::Player() 
    : playerHealth(100), currentWeapon(nullptr) {    // Initialize the player's properties
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
    playerHealth.TakeDamage(55);
}

void Player::handleInput() {
    vel[0] = 0.0f;
    vel[1] = 0.0f;

    const float speed = 4.0f;

    if (gl.keys[XK_Left] || gl.keys[XK_a]) vel[0] = -speed;
    if (gl.keys[XK_Right] || gl.keys[XK_d]) vel[0] = speed;
    if (gl.keys[XK_Up] || gl.keys[XK_w]) vel[1] = speed;
    if (gl.keys[XK_Down] || gl.keys[XK_s]) vel[1] = -speed;

    if (gl.keys[XK_space]) shoot(); // Shoot bullet when space is pressed

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


    // // Health GUI
    // std::string healthText = "Health: " + std::to_string(playerHealth.GetCurrentHealth()) + "/" + std::to_string(playerHealth.GetMaxHealth());
	// Rect r;
	// //glClear(GL_COLOR_BUFFER_BIT);
	// //
	// r.bot = gl.yres - 20;
	// r.left = 10;
	// r.center = 0;
    // ggprint8b(&r, 16, 0x00ff0000, healthText.c_str());
    // glEnd();

    int maxSegments = playerHealth.GetMaxHealth() / 10; // 10 segments of 10 health each
    float segmentWidth = 25.0f; // Width of each segment
    float segmentHeight = 30.0f; // Height of each segment
    float gap = 15.0f; // Gap between segments

    float slant = 10.0f; // Slant for the parallelogram

    float totalWidth = maxSegments * (segmentWidth + gap);

    float startX = gl.xres - totalWidth - 25; // Start position (10 units from the right edge)
    float startY = gl.yres - segmentHeight - 25; // Start position (10 units from the top edge)

    int filledSegments = playerHealth.GetCurrentHealth() / 10;

    for (int i = maxSegments - 1; i >= 0; i--) { // Starting from rightmost segment
        // Calculate segment position
        float x = startX + i * (segmentWidth + gap);
        float y = startY;

        // Determine color
        if ((maxSegments - i) <= filledSegments) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red for filled segment
        } else {
            glColor3f(0.5f, 0.5f, 0.5f); // Dark gray for empty segment
        }

        // Render segment as parallelogram
        glBegin(GL_QUADS);
            glVertex2f(x - slant, y); // Top-left
            glVertex2f(x + segmentWidth, y); // Top-right
            glVertex2f(x + segmentWidth + slant, y + segmentHeight); // Bottom-right
            glVertex2f(x, y + segmentHeight); // Bottom-left
        glEnd();
    }

    // Render the partial fill for the last segment if needed
    float partialFill = playerHealth.GetCurrentHealth() % 10;
    if (partialFill > 0) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for filled segment
        float partialHeight = (partialFill / 10.0) * segmentHeight;
        float x = startX + (maxSegments - filledSegments) * (segmentWidth + gap);
        float y = startY + segmentHeight - partialHeight;
        glBegin(GL_QUADS);
            glVertex2f(x - slant, y); // Top-left
            glVertex2f(x + segmentWidth, y); // Top-right
            glVertex2f(x + segmentWidth + slant, y + partialHeight); // Bottom-right
            glVertex2f(x, y + partialHeight); // Bottom-left
        glEnd();
    }


    // Weapon GUI
    std::string weaponText = "Equipped: " + currentWeapon->name;
	Rect weaponFont;
	//glClear(GL_COLOR_BUFFER_BIT);
	//
	weaponFont.bot = gl.yres - 40;
	weaponFont.left = 10;
	weaponFont.center = 0;
    ggprint8b(&weaponFont, 16, 0x00ff0000, weaponText.c_str());
    glEnd();
    //printf("%s",currentGun->name);


}



void Player::updateDirection(float mouseX, float mouseY) {
    angle = atan2(mouseY - pos[1], mouseX - pos[0]);
}


void Player::equipWeapon(Weapon* weapon) {
        currentWeapon = weapon;
    }

void Player::shoot() {
    if (currentWeapon) {
        currentWeapon->shoot(pos[0], pos[1], angle);
    } else {
        printf("No weapon equipped!\n");
    }
}


float Player::getX() const { return pos[0]; }
float Player::getY() const { return pos[1]; }
