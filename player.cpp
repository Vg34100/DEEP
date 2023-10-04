//player/player.cpp
#include "player.h"
#include "initGLX.h"
#include "prodriguezqu.h"

float easeOut(float factor) {
    return 1 - (1 - factor) * (1 - factor);
} 

void Player::cameraSetup() {
    // Camera lag
    // In the render function, adjust the camera lag:
    float factor = 0.1f;  // You can adjust this value as needed
    cameraPos.x += (playerPos.x - cameraPos.x) * easeOut(factor);
    cameraPos.y += (playerPos.y - cameraPos.y) * easeOut(factor);

    glPushMatrix(); 
    glTranslatef(-cameraPos.x, -cameraPos.y, 0.0f);

    // float maxCursorInfluenceDistance = 100.0f;
    
    // // Calculate the vector from the player to the mouse cursor
    // Vector2 toCursor = mousePos - playerPos;
    
    // // Calculate how much the cursor should affect the camera's position
    // float cursorInfluence = toCursor.length() / maxCursorInfluenceDistance;
    // cursorInfluence = std::min(1.0f, cursorInfluence);  // Ensure it's in [0, 1]

    // // Interpolate between the player's position and the cursor's position
    // Vector2 targetCameraPos = playerPos + toCursor * cursorInfluence;

    // // Smoothly move the camera towards the target position
    // cameraPos.x += (targetCameraPos.x - cameraPos.x) * easeOut(factor);
    // cameraPos.y += (targetCameraPos.y - cameraPos.y) * easeOut(factor);

    // glPushMatrix(); 
    // glTranslatef(-cameraPos.x, -cameraPos.y, 0.0f);
    
}


// void Player::handleMovement(float speedY, float speedX) {
//     // printf("handleMovement");
//     // fflush(stdout);
//     playerVelocity.y = speedY;
//     playerVelocity.x = speedX;
// }

void Player::handleInput() {

    playerVelocity.x = 0.0f;  // Reset horizontal velocity
    playerVelocity.y = 0.0f;  // Reset vertical velocity

    // Adjust velocity based on pressed keys
    if (keysPressed[XK_w]) playerVelocity.y += 25.0f * Speed;
    if (keysPressed[XK_s]) playerVelocity.y -= 25.0f * Speed;
    if (keysPressed[XK_a]) playerVelocity.x -= 25.0f * Speed;
    if (keysPressed[XK_d]) playerVelocity.x += 25.0f * Speed;

    // Normalize diagonal movement
    if ((playerVelocity.x != 0.0f) && (playerVelocity.y != 0.0f)) {
        float factor = sqrt(2.0f) / 2.0f;  // Equivalent to 1/sqrt(2)
        playerVelocity.x *= factor;
        playerVelocity.y *= factor;
    }

    if (keysPressed[XK_r]) useWeapon();
}


//KEEP - I guess this is important 
bool Player::isColliding() {
    Vector2 potentialNewPosition = playerPos;
    potentialNewPosition.add(playerVelocity);

    return collisionManager.isCollidingWithTile(potentialNewPosition, playerWidth/2 - 15.0f, playerHeight/2 - 15.0f);
}

void Player::showHitbox() const {
    // Draw the rectangle outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 1.0f);  // Color it red for visibility
    glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
    glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
    glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
    glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
    glEnd();
}

void Player::render() { 
    playerHealth.SetHealth(40);
    updatePlayerDirection();
    updateMousePosition(mousex, mousey);
    Vector2 topLeft = Vector2(playerPos.x - 0.5f * playerWidth, playerPos.y + 0.5f * playerHeight);
    Vector2 bottomRight = Vector2(playerPos.x + 0.5f * playerWidth, playerPos.y - 0.5f * playerHeight);
    Vector2 topRight = Vector2(playerPos.x + 0.5f * playerWidth, playerPos.y + 0.5f * playerHeight);
    Vector2 bottomLeft = Vector2(playerPos.x - 0.5f * playerWidth, playerPos.y - 0.5f * playerHeight);

    if (activeWeapon) {
        std::string Weapontext = activeWeapon->getName();
        // drawText(Weapontext.data(), Weapontext.size(), -width + 100, height-100, 1);
        activeWeapon->render();
    }


    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex2f(bottomLeft.x, bottomLeft.y); //bottomLeft corner
    glVertex2f(bottomRight.x, bottomRight.y); //bottomRight corner
    glVertex2f(topRight.x, topRight.y); //topRight corner
    glVertex2f(topLeft.x, topLeft.y); //topLeft corner

    glEnd();

    // Scale the direction for the distance of the indicator from the player
    float distanceFromPlayer = 50.0f;  // You can adjust this value
    Vector2 direction = playerDirection.multiply(distanceFromPlayer);

    // Compute the position of the indicator
    Vector2 indicatorPos = playerPos;
    indicatorPos.add(direction);

    // Draw the indicator
    float indicatorSize = 20.0f;  // This can be adjusted
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);  // Green color for the indicator
    glVertex2f(indicatorPos.x - 0.5f * indicatorSize, indicatorPos.y - 0.5f * indicatorSize);
    glVertex2f(indicatorPos.x + 0.5f * indicatorSize, indicatorPos.y - 0.5f * indicatorSize);
    glVertex2f(indicatorPos.x + 0.5f * indicatorSize, indicatorPos.y + 0.5f * indicatorSize);
    glVertex2f(indicatorPos.x - 0.5f * indicatorSize, indicatorPos.y + 0.5f * indicatorSize);
    glEnd();



    playerHealth.DisplayStaticHealthBar(-width + 100, height - 70);



    hitbox.topLeft = topLeft; 
    hitbox.bottomRight = bottomRight;
    #ifdef DEBUG     
    showHitbox();
    #endif

    glPopMatrix();
    showcaseHealth(playerHealth.GetMaxHealth(), playerHealth.GetCurrentHealth());
    glPushMatrix();

}

void Player::updateMousePosition(float mouseX, float mouseY) {
    mousePos.x = mouseX;
    mousePos.y = mouseY;
}

void Player::updatePlayerDirection() {
    playerDirection = Vector2(mousex - width / 2, height / 2 - mousey);
    playerDirection.normalize();
}



void Player::switchWeapon(int inventoryIndex) {
    activeWeapon = inventory[inventoryIndex];
}

void Player::useWeapon() { if (activeWeapon) { activeWeapon->use(); }}