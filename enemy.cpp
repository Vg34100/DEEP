// enemies/enemy.cpp
#include "enemy.h"
#include <ctime>

Enemy::Enemy(const std::string& nm, float dmg, float cooldown, float hp, float sz, World& wrld, Vector2 pos) 
	: name(nm), attackDamage(dmg), attackCooldown(cooldown), health(hp), size(sz), position(pos), world(wrld)  {
		#ifdef DEBUG
		std::cerr << "Enemy created: " << this << std::endl;
		#endif // DEBUG
 
	}

void Enemy::render() {
	// Rendering logic for the base enemy, e.g., drawing a simple colored square
}

void Enemy::attack() {
	// Basic attack logic, can be overridden by subclasses
}

void Enemy::summon(Vector2 pos) {
	if (canBeSummoned(pos)) {
		position = pos;
	} else {
		// Handle summoning to an invalid position, e.g., display a message
	}
}

bool Enemy::canBeSummoned(Vector2 pos) {
	return world.getTileTypeAtPosition(pos.x, pos.y) == TileType::ENEMY;  // Assuming ENEMY is an enum value in your World class
}


void Enemy::TakeDamage(float damage) {
	if (!invulnerable) {
		health.TakeDamage(damage);
		invulnerable = true; 
		lastDamageTime = std::clock(); 
		if(isDeadCheck()) { 
			enemiesDefeated++;
			enemiesRemaining--;
			isDead = true;
			deathTimestamp = std::clock();
		}
	}
}

void Enemy::UpdateInvulnerability() {
	if (invulnerable) {
		float elapsedTime = (std::clock() - lastDamageTime) / CLOCKS_PER_SEC;
		if (elapsedTime >= invincibilityDuration) {
			invulnerable = false;
		}
	}
}

void Enemy::showHitbox() const {
	if(isDead)
		return;
	// Draw the rectangle outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);  // Color it red for visibility
	glVertex2f(hitbox.topLeft.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.topLeft.y);
	glVertex2f(hitbox.bottomRight.x, hitbox.bottomRight.y);
	glVertex2f(hitbox.topLeft.x, hitbox.bottomRight.y);
	glEnd();
	return;
}

bool Enemy::isDeadCheck() const {
	return health.GetCurrentHealth() <= 0;
}

Hitbox Enemy::getHitbox() {  return hitbox; }



//name, damage, cooldown, hp, size, world
Orc::Orc(World& wrld, Vector2 pos) : Enemy("Orc", 20.0f, 1.0f, 100.0f, 128.0f, wrld, pos) {}

void Orc::render() {
	// if(isDead)
	//     return;
	UpdateInvulnerability();
	// Specific rendering for the orc
	// Define the color for the Orc - black in this case
	const float ORC_COLOR[] = {1.0f, 1.0f, 1.0f};

	// Apply the color
	glColor3fv(ORC_COLOR);

	// Assuming the position vector denotes the bottom-left of the square
	float leftX = position.x;
	float rightX = position.x + size;
	float bottomY = position.y;
	float topY = position.y + size;

	// Draw the square
	if(!isDead)
	{
		glBegin(GL_QUADS);
			glVertex2f(leftX, bottomY);
			glVertex2f(rightX, bottomY);
			glVertex2f(rightX, topY);
			glVertex2f(leftX, topY);
		glEnd();

		health.DisplayHealthBar(leftX + size/2, topY + 8);
	}


	Vector2 topLeft = Vector2(leftX, topY);
	Vector2 bottomRight = Vector2(rightX, bottomY);
	hitbox.topLeft = topLeft;
	hitbox.bottomRight = bottomRight;
	#ifdef DEBUG
	showHitbox();
	#endif
}

void Orc::attack() {
	// Specific attack logic for the orc
}