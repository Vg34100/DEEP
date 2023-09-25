//gui/titeScreen.cpp
#include "titleScreen.h"
#include "../setup/initGLX.h"

void titleScreen(float windowWidth, float windowHeight) {
    //Draw Background
    glColor3f(0.294f, 0.318f, 0.42f); 
    glBegin(GL_QUADS);
        glVertex2f(-windowWidth, -windowHeight);
        glVertex2f(windowWidth, -windowHeight);
        glVertex2f(windowWidth, windowHeight);
        glVertex2f(-windowWidth, windowHeight);
    glEnd();

    glColor3f(0.282f, 0.133f, 0.729f); 
    std:: string titleText = "DEEP";
    // drawText(titleText.data(), titleText.size(), 0 , 300, 5, 49);

    // Draw "Play" button
    glColor3f(0.059f, 0.106f, 0.188f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.31f * windowWidth, -0.11f * windowHeight);
        glVertex2f(0.31f * windowWidth, -0.11f * windowHeight);
        glVertex2f(0.31f * windowWidth, 0.11f * windowHeight);
        glVertex2f(-0.31f * windowWidth, 0.11f * windowHeight);
    glEnd();
    glColor3f(0.086f, 0.157f, 0.29f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.3f * windowWidth, -0.1f * windowHeight);
        glVertex2f(0.3f * windowWidth, -0.1f * windowHeight);
        glVertex2f(0.3f * windowWidth, 0.1f * windowHeight);
        glVertex2f(-0.3f * windowWidth, 0.1f * windowHeight);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);  
    std:: string playText = "PLAY";
    // drawText(playText.data(), playText.size(), 0 , -20, 1.5);

    // Draw "Settings" button
    glColor3f(0.059f, 0.106f, 0.188f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.31f * windowWidth, -0.41f * windowHeight);
        glVertex2f(0.31f * windowWidth, -0.41f * windowHeight);
        glVertex2f(0.31f * windowWidth, -0.19f * windowHeight);
        glVertex2f(-0.31f * windowWidth, -0.19f * windowHeight);
    glEnd();
    glColor3f(0.086f, 0.157f, 0.29f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.3f * windowWidth, -0.4f * windowHeight);
        glVertex2f(0.3f * windowWidth, -0.4f * windowHeight);
        glVertex2f(0.3f * windowWidth, -0.2f * windowHeight);
        glVertex2f(-0.3f * windowWidth, -0.2f * windowHeight);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);  
    std:: string settingText = "SETTINGS";
    // drawText(settingText.data(), settingText.size(), 0 , -225, 1.5);


    std:: string continueText = "Press enter to continue...";
    // drawText(continueText.data(), continueText.size(), 0 , -500, 0.9);

}