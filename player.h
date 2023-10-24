//player/player.h
#pragma once
#include <X11/Xlib.h>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "util_vector2.h"
#include "image.h"
#include "util_health.h"

#include "util_CollisionManager.h"

#include "vto.h"

//class World;

class Player {
private:
	CollisionManager& collisionManager; 
	// Attributes
	Health playerHealth;
	// Mana playerMana;
	// Ammo playerAmmo; ?
	// sanity? - or other major attribute
	float Speed; // speed multiplier - how fast the character moves
	float Damage; // damage(done) multiplier
	float Resistance; // damage(taken) multiplier
	float AttackSpeed; // rate of fire - how long is its cooldown
	float Range; // the weapons attack range - how long is its duration
	float Luck; // affects chace-based effects - idk what use for yet
	float Size; // cause idk, maybe u grew up
	float playerWidth;  // in world units
	float playerHeight; // in world units



	float invincibilityDuration = 0.1f; 
	bool invulnerable = false; 
	float lastDamageTime = 0.0f; 


	Vector2 playerPos;
	Vector2 mousePos;
	Vector2 cameraPos;
	Vector2 playerVelocity;
	Vector2 playerDirection; // Direction in which the player is looking
	std::shared_ptr<Weapon> activeWeapon;  // Using smart pointers for safer memory management
	Hitbox hitbox;

	Image idle{"images/player1_idle.png"};
	int directionx = 0;
	int directiony = 0;
    int currentFrame = 0;
    int totalFrames = 4;
    int frameDelay = 150;
    int timeSinceLastFrame = 0;
    static constexpr const char* SPRITE_SHEET_PATH = "images/player1_idle.png";




public:
	Player(CollisionManager& cm, float initialMaxHealth) : collisionManager(cm), playerHealth(initialMaxHealth) {
		activeWeapon = std::make_shared<Knife>(this);
		Speed = 1.0f;
		Damage = 1.0f;
		Resistance = 1.0f;
		AttackSpeed = 1.0f;
		Range = 1.0f;
		Luck = 1.0f;
		Size = 1.0f;
		playerWidth = 50.0f * Size;
		playerHeight = 50.0f * Size;
		activeWeapon->setDamage(activeWeapon->getDamage() * Damage);  // changes the weapon's damage based on player's damage stat
		if(activeWeapon->getWeaponClass() != "Melee")
			activeWeapon->setDuration(activeWeapon->getDuration() * Range); // changes the weapon's duration based on player's range stat | not for Melee Class
		activeWeapon->setCooldown(activeWeapon->getCooldown() / AttackSpeed); // changes the weapon's cooldown based on player's attack speed stat
		activeWeapon->setAttackSize(activeWeapon->getAttackSize() * Size); // change the weapon's attack size based on player's own size
		initialize();
	};

	std::vector<std::shared_ptr<Weapon>> inventory;

	//Getters and Setters
	Vector2 getPos() const { return playerPos; };
	Vector2 getVelocity() const { return playerVelocity; };
	Vector2 getDirection() const { return playerDirection; }; 
	float getPlayerWidth() { return playerWidth; };
	float getPlayerHeight() { return playerHeight; };
	std::shared_ptr<Weapon> getActiveWeapon() const { return activeWeapon; }

	void setPos(const Vector2& pos) { playerPos = pos; };
	void setX(float x) { playerPos.x = x; };
	void setY(float y) { playerPos.y = y; };


	void cameraSetup();
	void handleInput();
	void updateMousePosition(float mouseX, float mouseY);
	void updatePlayerDirection();  // Update the direction the player is looking based on mousePos

	bool initialize();
	void render();
    void animate(int elapsedTime);

	void handleMovement(float speedY, float speedX);
	void switchWeapon(int inventoryIndex);
	void useWeapon();

	void showHitbox() const;
	Hitbox getHitbox() { return hitbox; }


	void TakeDamage(float damage);
	void UpdateInvulnerability();

};


extern bool statsScreen;