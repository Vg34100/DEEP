#pragma once
#include <string>
void showcaseHealth(int maxHealth, int currentHealth);
int titleScreen();
int optionScreen();
extern int mouse_since;
int running_time(int& since, const bool get);
void levelenemyText(double elapsedtime);
void renderRect(std::string text, int value);
