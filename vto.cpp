//Author: Vincent To
//Weapons
#include "vto.h"
#include "player.h"
#include <ctime>
#include "prodriguezqu.h"
#include "image.h"

Vector2 rotatePoint(const Vector2& point, 
		float angle, const Vector2& center) 
{
	float s = sin(angle);
	float c = cos(angle);

	// Translate point back to the origin
	Vector2 p = point - center;

	// Rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// Translate point back to original position
	p.x = xnew + center.x;
	p.y = ynew + center.y;
	return p;
}

void Weapon::showHitbox() const 
{
	// Draw the rectangle outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);  // Color it red for visibility
	glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
	glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
	glEnd();
}

Lightsaber::Lightsaber(Player *p) : player(p) 
{
	name = "Lithium Ion Grip Heat Transfer-Saber";
	damage = 50; 
	cooldown = 0.5;
	attackSize = 1;
	speed = 0;
	duration = 1;
	damageType = "default";
	weaponClass = "Melee";
	if (!idle.loadTexture()) {
		std::cerr << "Failed to load texture" << std::endl;
	}
	idle.setSpriteSheet(1,2);
}

void Lightsaber::use() 
{
	if (!onCooldown && !isAttacking) {
		isAttacking = true;
		//lastUseTime = elapsedTime;  // Record the current time
	}
}

void Lightsaber::update(double elapsedTime) 
{
	static float lastAttackTime = 0.0;
	static float lastCooldownTime = 0.0;
	if (isAttacking) {
		lastAttackTime += elapsedTime;

		// Check if the attack duration has passed
		if (lastAttackTime >= duration) {
			isAttacking = false;
			onCooldown = true;
			lastUseTime = elapsedTime; // Reset the attack timer
			lastAttackTime = 0.0;
			lastCooldownTime = 0.0; // Start cooldown timer
		}
	} else if (onCooldown) {
		lastCooldownTime += elapsedTime;

		// Check if the cooldown has passed
		if (lastCooldownTime >= cooldown) {
			onCooldown = false;
			lastCooldownTime = 0.0; // Reset the cooldown timer
		}
	}
}


void Lightsaber::render() 
{
	Vector2 direction = player->getDirection();
	Vector2 position = player->getPos();

	float distanceFromPlayer = 120.0f;  
	Vector2 startPos = position + direction.multiply(distanceFromPlayer);
	float indicatorWidth = 250.0f * attackSize;
	float indicatorHeight = 200.0f * attackSize; 
	if (!isAttacking) {
		idle.renderSprite(0, 2, startPos.x, startPos.y, 100);
	}
	if (isAttacking) {
		idle.renderSprite(0, 1, startPos.x, startPos.y, 100);
	}

	Vector2 topLeft = Vector2(startPos.x - 0.2f * indicatorWidth, 
			startPos.y - 0.4f * indicatorHeight);
	Vector2 bottomRight = Vector2(startPos.x + 0.2f * indicatorWidth, 
			startPos.y + 0.4f * indicatorHeight);

	hitbox.topLeft = topLeft;
	hitbox.bottomRight = bottomRight;
#ifdef DEBUG
	showHitbox();
#endif
}

Mjolnir::Mjolnir(Player *p) : player(p) 
{
	name = "Mjolnir";
	damage = 50; 
	cooldown = 0.025;
	attackSize = 1;
	speed = 5;
	duration = 1;
	damageType = "default";
	weaponClass = "Range";
	if (!idle.loadTexture()) {
		std::cerr << "Failed to load texture" << std::endl;
	}
	idle.setSpriteSheet(1,2);
}

void Mjolnir::use() 
{
	if (!onCooldown && !isAttacking) {
		isAttacking = true;
		//lastUseTime = elapsedTime;  // Record the current time
	}
}

void Mjolnir::update(double elapsedTime) 
{
	static float lastAttackTime = 0.0;
	static float lastCooldownTime = 0.0;
	if (isAttacking) {
		lastAttackTime += elapsedTime;

		// Check if the attack duration has passed
		if (lastAttackTime >= duration) {
			isAttacking = false;
			onCooldown = true;
			lastUseTime = elapsedTime; // Reset the attack timer
			lastAttackTime = 0.0;
			lastCooldownTime = 0.0; // Start cooldown timer
		}
	} else if (onCooldown) {
		lastCooldownTime += elapsedTime;

		// Check if the cooldown has passed
		if (lastCooldownTime >= cooldown) {
			onCooldown = false;
			lastCooldownTime = 0.0; // Reset the cooldown timer
		}
	}

}

void Mjolnir::render() 
{
	//update();
	Vector2 direction = player->getDirection();
	Vector2 position = player->getPos();

	float distanceFromPlayer = 120.0f;  
	Vector2 startPos = position + direction.multiply(distanceFromPlayer);
	float indicatorWidth = 250.0f;
	float indicatorHeight = 200.0f; 

	if (!isAttacking) {
		idle.renderSprite(0, 2, startPos.x, startPos.y, 100);
	}
	if (isAttacking) {
		idle.renderSprite(0, 1, startPos.x, startPos.y, 100);
	}
	Vector2 topLeft = Vector2(startPos.x - 0.5f * indicatorWidth, 
			startPos.y - 0.5f * indicatorHeight);
	Vector2 bottomRight = Vector2(startPos.x + 0.5f * indicatorWidth, 
			startPos.y + 0.5f * indicatorHeight);

	hitbox.topLeft = topLeft;
	hitbox.bottomRight = bottomRight;
#ifdef DEBUG
	showHitbox();
#endif
}


//Statistics for game
int renderFunctionCalls(const bool renderFun) 
{
	static int actualCount = 1;
	if (renderFun) {
		actualCount++;
		renderRect("Redner has been called: ", actualCount);
	} else {
		return 0;
	}
	return 0 ;
}

bool holdtoPress(bool reset)
{
	static int DMkeyHoldCounter = 0; //DM - DevMode
	if(reset)
		DMkeyHoldCounter = 0;
	const int DMkeyHoldThreshold = 30; 
	if (DMkeyHoldCounter <= 0) {
			DMkeyHoldCounter = DMkeyHoldThreshold;  // Reset the counter when state changes
			return true;
	} else {
			DMkeyHoldCounter--;  // Decrease the counter if key is held
			return false;
	}
	return false;
}

bool handleKeyAction(bool keyCondition, int &counter, int threshold) 
{
	if (keyCondition) {
		if (counter <= 0) {
			counter = threshold;  // Reset the counter
			return true;  // Indicate that the action should be performed
		} else {
			counter--;  // Decrease the counter if key is held
		}
	} else {
		counter = 0;  // Reset the counter if key is not pressed
	}
	return false;  // Indicate that the action should not be performed
}