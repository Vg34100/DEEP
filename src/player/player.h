// player.h
#ifndef PLAYER_H
#define PLAYER_H

typedef float Vec[3];

class Player {
private:
    Vec pos;  // Player's position
    Vec vel;  // Player's velocity
    float color[3];  // Player's color
    float radius;
    float angle;

public:
    Player();
    void handleInput();  // Handle user inputs to update velocity
    void update();  // Update player's position based on velocity
    void render();  // Render the player on screen
    void updateDirection(float mouseX, float mouseY);

};

#endif
