#pragma once

void showcaseHealth(int maxHealth, int currentHealth);
int titleScreen();
int optionScreen();
extern int mouse_since;
int running_time(int& since, const bool get);
void levelenemyText(double elapsedtime);