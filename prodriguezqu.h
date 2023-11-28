#pragma once
#include <string>

class Player;
class Numbers;
class Letters;

void renderHealthBar(int maxHealth, int currentHealth);
int titleScreen();
int optionScreen();
int running_time(int& since, const bool get);
int mouse_since_counter(const bool reset, bool render);
void levelenemyText(double elapsedtime);
void renderRect(std::string text, int value);
bool on_level_complete(float deltaTime);
void on_level_enter();
bool stats_screen(Player& player, Numbers& numbers, float deltaTime);
bool constant_stats(Player& player, Letters& letters, Numbers& numbers, float deltaTime);
int paused();


