// health.h

#pragma once

class Health {
public:
    Health(int max_hp);

    void TakeDamage(int damage);
    void Heal(int amount);
    
    bool IsDead() const;
    
    int GetCurrentHealth() const;
    void SetMaxHealth(int new_max);
    int GetMaxHealth() const;

private:
    int max_health;
    int current_health;
};
