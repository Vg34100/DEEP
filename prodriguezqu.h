#pragma once
#include <string>
#include <functional>

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

void processMenuInput(
    bool inputCondition, 
    int& keyHoldCounter, 
    std::function<void()> actionFunction);

template<typename T>
void changeState(T& current, int direction) 
{
    // Calculate the number of states in the enum
    int numStates = static_cast<int>(T::MAX_STATES);

    // Convert enum class to int, adjust the state, and handle wrapping 
    int newState = (static_cast<int>(current) + direction + numStates) % numStates;
    
    // Convert back to the enum type and assign it to the reference
    current = static_cast<T>(newState);
}