// enemies/enemy.h
#pragma once

#include "util_health.h"
#include <string>
#include "util_vector2.h"
#include "world.h"

class World;

class Enemy {
protected:
	float deathTimestamp = 0.0f;

	std::string name;
	float attackDamage;
	float attackCooldown;
	Health health;  
	float size;
	Vector2 position;  
	World& world; 
	float invincibilityDuration = 0.1f; 
	bool invulnerable = false; 
	float lastDamageTime = 0.0f; 
	Hitbox hitbox;
	bool isDead = false;

public:
	Enemy(const std::string& name, float dmg, float cooldown, float hp, float sz, World& wrld, Vector2 pos);
	~Enemy() {
	std::cerr << "Enemy destroyed: " << this << std::endl;
	}

	virtual void render();  // For drawing the enemy
	virtual void attack();  // Attack action, can be overridden by subclasses
	
	void summon(Vector2 pos);  // Summon the enemy to a specific position
	bool canBeSummoned(Vector2 pos);  // Check if the enemy can be summoned to a specific tile
	Vector2 getPosition() { return position; }
	void TakeDamage(float damage);
	void UpdateInvulnerability();

	void showHitbox() const;
	Hitbox getHitbox();
	float getDeathTimestamp() { return deathTimestamp; }
	bool isDeadCheck() const;
};

class Orc : public Enemy {
public:
	Orc(World& wrld, Vector2 pos);
	
	void render() override;
	void attack() override;
};


