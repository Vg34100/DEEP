#pragma once
#include "image.h"
#include <string>
#include "accessories.h"
#include <functional>

class Player;
class World;
class Image;

class Letters {
private:
	Image letters{"images/letters.png"};
public:
	Letters();
	bool init();
    void renderLetters(const std::string& str, float x, float y, float scale);
};

class Numbers {
private:
	Image numbers{"images/number.png"};

public:
	Numbers();  // Constructor declaration
	bool init();
	void renderNumbers(float num, float x, float y, float scale);
	void renderInts(int num, float x, float y, float scale);
};

class GUI {
private:
	World& world;
	Player& player;
	Numbers numbers;
	Letters letters;
public:
	// Updated constructor
	GUI(World& world, Player& player);

	void render(float deltaTime);
	bool badge();
};

class Shop {
private:
	World& world;
	Player& player;
	Numbers numbers;
	Letters letters;
	AccessoryCollection collection; // Make this a member of the Shop class
	size_t selectedIndex;

public:
	// Updated constructor
	Shop(World& world, Player& player);

	void render(float deltaTime);
};

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