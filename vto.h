//vto.h
#pragma once
#include <string>
#include "util_vector2.h"
#include "image.h"

int renderFunctionCalls(const bool renderFun);
class Player;

class Weapon {
public:
	// Getters
	std::string getName() const { return name; }
	int getDamage() const { return damage; }
	float getCooldown() const { return cooldown; }
	float getAttackSize() const { return attackSize; }
	float getSpeed() const { return speed; }
	float getDuration() const { return duration; }
	std::string getDamageType() const { return damageType; }
	std::string getWeaponClass() const { return weaponClass; }
	bool getIsAttacking() { return isAttacking; }

	// Setters (for properties that might change, for example through upgrades)
	void setName(const std::string &n) { name = n; }
	void setDamage(int d) { damage = d; }
	void setCooldown(float c) { cooldown = c; }
	void setAttackSize(float a) { attackSize = a; }
	void setDuration(float d) { duration = d; }

	virtual void use() = 0; // Pure virtual function to ensure each derived class implements it.
	virtual void render() = 0;
	virtual void update() = 0;
	// void setHitbox(const Vector2& topLeft, const Vector2& bottomRight) {
	//     hitbox.topLeft = topLeft;
	//     hitbox.bottomRight = bottomRight;
	// }
	void showHitbox() const;
	Hitbox getHitbox() { return hitbox; }
	// virtual Hitbox getHitbox() const = 0;

protected:
	// Additional private or protected attributes/methods
	std::string name;
	int damage;
	float cooldown;
	float attackSize;
	float speed;
	float duration;
	std::string damageType;
	std::string weaponClass;
	bool isAttacking = false; // if currently attacking
	bool onCooldown = false; // if on cooldown
	float lastUseTime = 0; // timestamp of last use

	Hitbox hitbox;
};


class MeleeWeapon : public Weapon {
public:
	MeleeWeapon() {
		weaponClass = "Melee";
		speed = 0;
	}

	void use() override {}
	void update() override {}
	void render() override {}
private:
	// Function to check for critical hits based on timing
	bool checkForCrit();
};

class RangedWeapon : public Weapon {
public:
	int ammo;
	RangedWeapon() {
		weaponClass = "Ranged";
	}
	void use() override {}

private:
	bool hasSufficientAmmo();
};

class MagicWeapon : public Weapon {
public:
	int manaCost;

	MagicWeapon() {
		weaponClass = "Magic";
	}

	void use() override {}

private:
	bool hasSufficientMana();
};

class SummonWeapon : public Weapon {
public:
	int maxSummons;

	SummonWeapon() {
		weaponClass = "Summon";
	}

	void use() override {}

private:
	int currentSummons();
};

class Lightsaber : public MeleeWeapon {
public:
	Lightsaber(Player *p);

	void use() override;

	void update() override;

	void render() override;
private:
	Player *player; // A pointer/reference to the player to know their direction and position
    Image idle{"images/lightsaberSprite.jpeg"};
	static constexpr const char* SPRITE_SHEET_PATH = "images/lightsaberSprite.jpeg";

};
class Gun : public RangedWeapon {
public:
	Gun(Player *p);

	void use() override;

	void update() override;

	void render() override;
private:
	Player *player; // A pointer/reference to the player to know their direction and position
};