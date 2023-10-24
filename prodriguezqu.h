#pragma once
#include <string>
void showcaseHealth(int maxHealth, int currentHealth);
int titleScreen();
int optionScreen();
int running_time(int& since, const bool get);
int mouse_since_counter(const bool reset, bool render);
void levelenemyText(double elapsedtime);
void renderRect(std::string text, int value);
