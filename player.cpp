//player/player.cpp
#include "player.h"
#include "initGLX.h"
#include "prodriguezqu.h"
#include <ctime>


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
	
}

void Player::handleInput() {

	playerVelocity.x = 0.0f;  // Reset horizontal velocity
	playerVelocity.y = 0.0f;  // Reset vertical velocity
	directiony = -1;
	directionx = -1;

	// Adjust velocity based on pressed keys
	if (keysPressed[XK_w]) {
		playerVelocity.y += 4.0f * Speed;
		directiony = 0;
	}
	if (keysPressed[XK_s]) {
		playerVelocity.y -= 4.0f * Speed;
		directiony = 1;
	} 
	if (keysPressed[XK_a]) {
		playerVelocity.x -= 4.0f * Speed;
		directionx = 0;
	} 
	if (keysPressed[XK_d]) {
		playerVelocity.x += 4.0f * Speed;
		directionx = 1;
	} 

	// Normalize diagonal movement
	if ((playerVelocity.x != 0.0f) && (playerVelocity.y != 0.0f)) {
		float factor = sqrt(2.0f) / 2.0f;  // Equivalent to 1/sqrt(2)
		playerVelocity.x *= factor;
		playerVelocity.y *= factor;
	}

	if (keysPressed[XK_r]) useWeapon();
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

bool Player::initialize() {
    // Perform complex initializations here (e.g., loading textures)
    if (!idle.loadTexture()) {
        std::cerr << "Failed to load texture" << std::endl;
        return false;
    }
    idle.setSpriteSheet(4,4);
    // ... other initialization code...
    return true;
}

void Player::animate(int elapsedTime) {
    timeSinceLastFrame += elapsedTime;

    if (timeSinceLastFrame > frameDelay) {
        currentFrame = (currentFrame + 1) % totalFrames;
        timeSinceLastFrame = 0;  // Reset the timer
    }
}

int determineSpriteRow(int directionsx, int directionsy) {
    // Using directionsy and directionsx to determine the row.
    if (directionsy == 0 && directionsx == 0) return 3; // Up left
    if (directionsy == 0 && directionsx == 1) return 3; // Up right
    if (directionsy == 1 && directionsx == 0) return 1; // Down left
    if (directionsy == 1 && directionsx == 1) return 1; // Down right
	if (directionsy == 0 && directionsx == -1) return 2;
	if (directionsy == 1 && directionsx == -1) return 0;
	if (directionsy == -1 && directionsx == 0) return 1;
	if (directionsy == -1 && directionsx == 1) return 1;



    return 0; // Default, in case of invalid input
}

bool shouldFlipSprite(int directionsx) {
    // We flip the sprite only when the direction is left (either up-left or down-left).
    return directionsx == 0;
}

void Player::render() { 
	//playerHealth.SetHealth(40);
	UpdateInvulnerability();
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

	/*
	Directionsx
	0 - left
	1 - right
	Directionsy
	0 - up
	1 - down
	*/

	int row = determineSpriteRow(directionx, directiony);
	bool flip = shouldFlipSprite(directionx);

	idle.renderSprite(row, currentFrame, playerPos.x, playerPos.y, 42, flip);
    // idle.renderSprite(0, currentFrame, playerPos.x, playerPos.y, 42);


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

void Player::TakeDamage(float damage) {
	if (!invulnerable) {
		playerHealth.TakeDamage(damage);
		invulnerable = true; 
		lastDamageTime = std::clock(); 
	}
}

void Player::UpdateInvulnerability() {
	if (invulnerable) {
		float elapsedTime = (std::clock() - lastDamageTime) / CLOCKS_PER_SEC;
		if (elapsedTime >= invincibilityDuration) {
			invulnerable = false;
		}
	}
}