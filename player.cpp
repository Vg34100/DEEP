//player/player.cpp
#include "player.h"
#include "initGLX.h"
#include "prodriguezqu.h"
#include <ctime>
#include "keybinds.h"


float easeOut(float factor) 
{
	return 1 - (1 - factor) * (1 - factor);
} 

void Player::updateCameraShake() 
{
	if (shakeFrames > 0) {
		// Decrement the shake frame counter
		--shakeFrames;

		// Randomly adjust camera position
		float shakeX = ((std::rand() % 100) / 100.0f * 2.0f - 1.0f) * shakeIntensity;
		float shakeY = ((std::rand() % 100) / 100.0f * 2.0f - 1.0f) * shakeIntensity;

		cameraPos.x += shakeX;
		cameraPos.y += shakeY;
	}
}

void Player::startCameraShake(float intensity, int frames) 
{
	shakeIntensity = intensity;
	shakeFrames = frames;
}

void Player::cameraSetup() 
{
    float factor = 0.1f;  // Adjust this value as needed for player following
    float mouseFactor = 0.1f; // Adjust this value as needed for mouse influence

    // Update camera position to follow player
    cameraPos.x += (playerPos.x - cameraPos.x) * easeOut(factor);
    cameraPos.y += (playerPos.y - cameraPos.y) * easeOut(factor);

    // Calculate mouse influence
    float mouseXCentered = mousex - actual_width / 2.0f;
    float mouseYCentered = mousey - actual_height / 2.0f;

    // Apply a small translation to the camera based on mouse position
    cameraPos.x += mouseXCentered * mouseFactor;
    cameraPos.y += -mouseYCentered * mouseFactor;

    // Apply camera transformation
    glPushMatrix(); 
    glTranslatef(-cameraPos.x, -cameraPos.y, 0.0f);

	updateCameraShake();
}



void Player::handleInput() 
{
	playerVelocity.x = 0.0f;  // Reset horizontal velocity
	playerVelocity.y = 0.0f;  // Reset vertical velocity
	directiony = -1;
	directionx = -1;

	// Adjust velocity based on pressed keys
	if (keysPressed[move_up]) {
		playerVelocity.y += 4.0f * Speed;
		directiony = 0;
	}
	if (keysPressed[move_down]) {
		playerVelocity.y -= 4.0f * Speed;
		directiony = 1;
	} 
	if (keysPressed[move_left]) {
		playerVelocity.x -= 4.0f * Speed;
		directionx = 0;
	} 
	if (keysPressed[move_right]) {
		playerVelocity.x += 4.0f * Speed;
		directionx = 1;
	} 

	// Normalize diagonal movement
	if ((playerVelocity.x != 0.0f) && (playerVelocity.y != 0.0f)) {
		float factor = sqrt(2.0f) / 2.0f;  // Equivalent to 1/sqrt(2)
		playerVelocity.x *= factor;
		playerVelocity.y *= factor;
	}

	//if (keysPressed[attack_key]) useWeapon();
	if(keysPressed[attack_key] || leftMouseButtonPressed)
		useWeapon();
}

void Player::showHitbox() const 
{
	// Draw the rectangle outline
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);  // Color it red for visibility
	glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
	glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
	glEnd();
}

bool Player::initialize() 
{
    // Perform complex initializations here (e.g., loading textures)
    if (!idle.loadTexture()) {
        std::cerr << "Failed to load texture" << std::endl;
        return false;
    }
    idle.setSpriteSheet(4,4);
    // ... other initialization code...
    return true;
}

void Player::animate(int elapsedTime) 
{
    timeSinceLastFrame += elapsedTime;

    if (timeSinceLastFrame > frameDelay) {
        currentFrame = (currentFrame + 1) % totalFrames;
        timeSinceLastFrame = 0;  // Reset the timer
    }
}

int determineSpriteRow(int directionsx, int directionsy) 
{
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

bool shouldFlipSprite(int directionsx) 
{
    // We flip the sprite only when the direction is left (either up-left or down-left).
    return directionsx == 0;
}

void Player::render() 
{ 
	//playerHealth.SetHealth(40);
	UpdateInvulnerability();
	updatePlayerDirection();
	updateMousePosition(mousex, mousey);
	// printf("%f, %f\n", mousex, mousey);
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
	renderHealthBar(playerHealth.GetMaxHealth(), playerHealth.GetCurrentHealth());
	glPushMatrix();

}

void Player::updateMousePosition(float mouseX, float mouseY) 
{
	mousePos.x = mouseX;
	mousePos.y = mouseY;
}

void Player::updatePlayerDirection() 
{
	playerDirection = Vector2(mousex - actual_width / 2, actual_height / 2 - mousey);
	playerDirection.normalize();
}



void Player::switchWeapon(int inventoryIndex) 
{
	activeWeapon = inventory[inventoryIndex];
}

void Player::useWeapon() { if (activeWeapon) { activeWeapon->use(); }}

void Player::TakeDamage(float damage) 
{
	if (!invulnerable) {
		playerHealth.TakeDamage(damage - Resistance/damage + ((levelsCompleted/10) * damage));
		invulnerable = true; 
		lastDamageTime = std::clock(); 
		startCameraShake(20,25);
	}
}

void Player::UpdateInvulnerability() 
{
	if (invulnerable) {
		float elapsedTime = (std::clock() - lastDamageTime) / CLOCKS_PER_SEC;
		if (elapsedTime >= invincibilityDuration) {
			invulnerable = false;
		}
	}
}

void Player::updateStatwheel() 
{
	statWheel[0] = playerHealth.GetMaxHealth();
	statWheel[1] = playerMana;
	statWheel[2] = static_cast<float>(playerAmmo);
	statWheel[3] = Sanity;
	statWheel[4] = Speed;
	statWheel[5] = Damage;
	statWheel[6] = Resistance;
	statWheel[7] = AttackSpeed;
	statWheel[8] = Range;
	statWheel[9] = Luck;
	statWheel[10] = Size;
}

float Player::getStatwheel(int num)
{
	return statWheel[num];
}

void Player::randomlyIncrementAttribute(float minIncrement, float maxIncrement) 
{
	srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

	int attribute = rand() % 11; // Randomly select an attribute (0 to 10)
	int negation = rand() % 2;
	float increment = minIncrement + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxIncrement - minIncrement))); // Random increment

	switch (attribute) {
		case 0: 
			playerHealth.AddMaxHealth(increment*5);
			playerHealth.SetHealth(playerHealth.GetMaxHealth()); 
			break; 
		case 1: playerMana += increment; break;
		case 2: playerAmmo += static_cast<int>(increment); break; // Casting to int
		case 3: negation ? Sanity += increment * 3 : Sanity -= increment * 3; break;
		case 4: Speed += increment/20; break;
		case 5: 
			Damage += increment/25; 
			/* Add Setting Damage on Weapon */
			activeWeapon->setDamage(activeWeapon->getDamage() + Damage);
			break;
		case 6: Resistance += increment/10; break;
		case 7: 
			AttackSpeed += increment/150; 
			/* Add Setting AttackSpeed on Weapon */
			activeWeapon->setCooldown(activeWeapon->getCooldown() - AttackSpeed);
			break;
		case 8: 
			Range += increment/10; 
			/* Add Setting Range on Weapon */
			break;
		case 9: 
			Luck += increment/5; 
			/* Add Setting Luck on Weapon */
			break;
		case 10: 
			negation ? Size += increment/20 : Size -= increment/20; 
			activeWeapon->setAttackSize(Size);
			break;
		default: break; // In case of an unexpected value
	}
}

void Player::randomlyCollectCoins(float minIncrement, float maxIncrement)
{
	srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

	//int attribute = rand() % 11; // Randomly select an attribute (0 to 10)
	//int negation = rand() % 2;
	float increment = minIncrement + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxIncrement - minIncrement))); // Random increment
	coins += increment;

}

void Player::updateWeapon()
{
	activeWeapon->setDamage(activeWeapon->getDamage() * Damage);  // changes the weapon's damage based on player's damage stat
	if (activeWeapon->getWeaponClass() != "Melee")
		activeWeapon->setDuration(activeWeapon->getDuration() * Range); // changes the weapon's duration based on player's range stat | not for Melee Class
	activeWeapon->setCooldown(activeWeapon->getCooldown() / AttackSpeed); // changes the weapon's cooldown based on player's attack speed stat
	activeWeapon->setAttackSize(activeWeapon->getAttackSize() * Size); // change the weapon's attack size based on player's own size
}