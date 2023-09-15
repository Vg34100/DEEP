// health.cpp

#include "health.h"

Health::Health(int max_hp) : max_health(max_hp), current_health(max_hp) {}

void Health::TakeDamage(int damage) {
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }
}

void Health::Heal(int amount) {
    current_health += amount;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

bool Health::IsDead() const {
    return current_health <= 0;
}

int Health::GetCurrentHealth() const {
    return current_health;
}

void Health::SetMaxHealth(int new_max) {
    max_health = new_max;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

int Health::GetMaxHealth() const {
    return max_health;
}
