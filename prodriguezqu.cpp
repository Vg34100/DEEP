#include "prodriguezqu.h"
/**
 * Author: Pablo Rodriguez
 * Created: 9.29.23 
*/ 

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <iostream>
#include "initGLX.h"

void showcaseHealth(int maxHealth, int currentHealth) {
    // Ensure that currentHealth does not exceed maxHealth
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }

    // Screen dimensions, for example 800x600
    int screenWidth = width;
    int screenHeight = height;

    // Dimensions of the health bar
    int barWidth = 200; // for example
    int barHeight = 40; // for example

    // Choose a corner for the health bar, e.g., top-left
    int xPosition = screenWidth - barWidth - 10; // 10 pixels from the left
    int yPosition = screenHeight - barHeight - 10; // 10 pixels from the top

    // Calculate the width of the current health bar in relation to max health
    int currentBarWidth = (currentHealth * barWidth) / maxHealth;

    // Draw the full health bar
    glColor3f(0.1, 0.1, 0.1); // Red color
    glBegin(GL_QUADS);
        glVertex2i(xPosition, yPosition);
        glVertex2i(xPosition + barWidth, yPosition);
        glVertex2i(xPosition + barWidth, yPosition + barHeight);
        glVertex2i(xPosition, yPosition + barHeight);
    glEnd();

    // Draw the current health bar above the full health bar
    glColor3f(0.0, 1.0, 0.0); // Green color
    glBegin(GL_QUADS);
        glVertex2i(xPosition, yPosition);
        glVertex2i(xPosition + currentBarWidth, yPosition);
        glVertex2i(xPosition + currentBarWidth, yPosition + barHeight);
        glVertex2i(xPosition, yPosition + barHeight);
    glEnd();
}
