//Author: Vincent To
//Weapons
#include "vto.h"
#include "player.h"
#include <ctime>
#include "prodriguezqu.h"

Vector2 rotatePoint(const Vector2& point, float angle, const Vector2& center) {
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

void Weapon::showHitbox() const {
	// Draw the rectangle outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);  // Color it red for visibility
	glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
	glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
	glEnd();
}

Knife::Knife(Player *p) : player(p) {
	name = "Lithium Ion Grip Heat Transfer-Saber";
	damage = 200; 
	cooldown = 0.05;
	attackSize = 3;
	speed = 0;
	duration = 0.05;
	damageType = "default";
	weaponClass = "Melee";
}

void Knife::use() {
	if (!onCooldown && !isAttacking) {
		isAttacking = true;
		lastUseTime = std::clock();  // Record the current time
	}
}

void Knife::update() {
	if (isAttacking) {
		float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;
		
		// Check if the attack duration has passed
		if (elapsedTime >= duration) {
			isAttacking = false;
			onCooldown = true;
			lastUseTime = std::clock(); // Reset the timer for cooldown
		}
	} else if (onCooldown) {
		float elapsedTime = (std::clock() - lastUseTime) / CLOCKS_PER_SEC;
		
		// Check if the cooldown has passed
		if (elapsedTime >= cooldown) {
			onCooldown = false;
		}
	}
}

void Knife::render() {
	update();
	Vector2 direction = player->getDirection();
	Vector2 position = player->getPos();

	float distanceFromPlayer = 20.0f;  
	Vector2 startPos = position + direction.multiply(distanceFromPlayer);
	float indicatorWidth = 250.0f;
	float indicatorHeight = 200.0f; 
	if (std::abs(direction.x) > std::abs(direction.y)) {
		// Left or Right
		std::swap(indicatorWidth, indicatorHeight);
		if (direction.x > 0) { // Right
			startPos.x += 0.5f * indicatorWidth; // Adjust start position 
                                                 // slightly to the right
		} else { // Left
			startPos.x -= 0.5f * indicatorWidth; // Adjust start position 
                                                 // slightly to the left
		}
	} else {
		// Up or Down
		if (direction.y > 0) { // Up
			startPos.y += 0.5f * indicatorHeight; // Adjust start position 
                                                  // slightly upwards
		} else { // Down
			startPos.y -= 0.5f * indicatorHeight; // Adjust start position 
                                                  // slightly downwards
		}
	}
	if (isAttacking) {
		// Draw the rectangle
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex2f(startPos.x - 0.5f * indicatorWidth, startPos.y - 0.5f * indicatorHeight);
		glVertex2f(startPos.x + 0.5f * indicatorWidth, startPos.y - 0.5f * indicatorHeight);
		glVertex2f(startPos.x + 0.5f * indicatorWidth, startPos.y + 0.5f * indicatorHeight);
		glVertex2f(startPos.x - 0.5f * indicatorWidth, startPos.y + 0.5f * indicatorHeight);
		glEnd();
	}
	Vector2 topLeft = Vector2(startPos.x - 0.5f * indicatorWidth, startPos.y - 0.5f * indicatorHeight);
	Vector2 bottomRight = Vector2(startPos.x + 0.5f * indicatorWidth, startPos.y + 0.5f * indicatorHeight);
	
	// setHitbox(topLeft, bottomRight);
	hitbox.topLeft = topLeft;
	hitbox.bottomRight = bottomRight;
	#ifdef DEBUG
	showHitbox();
	#endif
}

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
