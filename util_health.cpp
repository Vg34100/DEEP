//common/health.cpp
#include "util_health.h"
#include <algorithm>
#include "initGLX.h"

Health::Health(float initialMaxHealth) : maxHealth(initialMaxHealth), currentHealth(initialMaxHealth),
	tempHealth(0.0f), regenRate(0.0f), isRegenerating(false), isInvulnerable(false) {}

void Health::TakeDamage(float damage, const std::string& damageType) 
{
	if (isInvulnerable) return;
	float resistance = GetResistance(damageType);
	float effectiveDamage = damage * (1.0f - resistance);
	currentHealth -= effectiveDamage;
	ShowDamageNumber(effectiveDamage);
}

void Health::Heal(float amount) 
{
	currentHealth = std::min(maxHealth, currentHealth + amount);
}

void Health::SetHealth(float amount) 
{
	currentHealth = amount;
}

void Health::AddMaxHealth(float amount)
{
	maxHealth += amount;
}

void Health::SetMaxHealth(float amount)
{
	maxHealth = amount;
}


void Health::AddTemporaryHealth(float amount) 
{
	tempHealth += amount;
	currentHealth += amount;
}

float Health::GetCurrentHealth() const 
{
	return currentHealth;
}

float Health::GetMaxHealth() const 
{
	return maxHealth;
}

bool Health::IsAlive() const 
{
	return currentHealth > 0;
}

void Health::StartInvulnerability(float duration) 
{
	// Ideally, you'd need a timer system to automatically end invulnerability after duration expires
	isInvulnerable = true;
}

void Health::EndInvulnerability() 
{
	isInvulnerable = false;
}

void Health::StartRegeneration(float rate) 
{
	regenRate = rate;
	isRegenerating = true;
}

void Health::StopRegeneration() 
{
	isRegenerating = false;
}

void Health::UpdateRegeneration() 
{
	if (isRegenerating) {
		currentHealth = std::min(maxHealth, currentHealth + regenRate);
	}
}

void Health::SetResistance(const std::string& damageType, float resistanceMultiplier) 
{
	damageResistances[damageType] = resistanceMultiplier;
}

float Health::GetResistance(const std::string& damageType) const 
{
	if (damageResistances.find(damageType) != damageResistances.end()) {
		return damageResistances.at(damageType);
	}
	return 0.0f; // No resistance for this type
}

void Health::ShowDamageNumber(float damageAmount) const 
{
	// This would involve some graphical or console output, so it's just a placeholder here.
	#ifdef DEBUG
	printf("Damage taken: %.2f\n", damageAmount);
	#endif // DEBUG
}

void Health::DisplayHealthBar(float positionX, float positionY) const 
{
	std:: string text = std::to_string(static_cast<int>(currentHealth)) + "/" + std::to_string(static_cast<int>(maxHealth));
}

void Health::DisplayStaticHealthBar(float positionX, float positionY) const 
{
	std:: string text = std::to_string(static_cast<int>(currentHealth)) + "/" + std::to_string(static_cast<int>(maxHealth));
}


