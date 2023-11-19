#pragma once
#include "image.h"
#include <string>

class Player;
class World;
class Image;

class Letters {
private:
    Image letters{"images/letters.png"};
public:
    Letters();
    bool init();
    void renderLetters(std::string& str, float x, float y, float scale);
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
    // GUI(World& world, Player& player) : world(world), player(player) {}
    GUI(World& world, Player& player);

    void render(float deltaTime);
    bool badge();
};

