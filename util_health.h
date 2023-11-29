//common/health.h
#pragma once

#include <unordered_map>
#include <string>

class Health {
private:
	float maxHealth;
	float currentHealth;
	float tempHealth; // Overhealing or Temporary Health
	float regenRate;
	bool isRegenerating;
	bool isInvulnerable;
	std::unordered_map<std::string, float> damageResistances; // Damage type to resistance multiplier

public:
	Health(float initialMaxHealth);

	// Basic Health Modification Methods
	void TakeDamage(float damage, const std::string& damageType = "default");
	void Heal(float amount);
	void SetHealth(float amount);
	void AddMaxHealth(float amount);
	void SetMaxHealth(float amount);
	void AddTemporaryHealth(float amount);

	// Getter methods
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	bool IsAlive() const;

	// Invulnerability
	void StartInvulnerability(float duration);
	void EndInvulnerability();

	// Regeneration
	void StartRegeneration(float rate);
	void StopRegeneration();
	void UpdateRegeneration(); // This would be called in a game loop or update method

	// Resistance
	void SetResistance(const std::string& damageType, float resistanceMultiplier);
	float GetResistance(const std::string& damageType) const;

	// Health UI and Effects
	// This will be basic because full implementation would depend on a graphics library or engine
	void ShowDamageNumber(float damageAmount) const; // Example: show a floating number
	void DisplayHealthBar(float positionX, float positionY) const; 
	void DisplayStaticHealthBar(float positionX, float positionY) const; 
};
