#include "accessories.h"
#include "player.h"

void HeartLocket::applyEffect(Player& player)  
{
    player.AddMaxHealth(50);
}

void ManaBottle::applyEffect(Player& player)  
{
    // player.AddMaxHealth(50);
}

void Magazine::applyEffect(Player& player)  
{
    // player.AddMaxHealth(50);
}