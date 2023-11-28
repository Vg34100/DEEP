#include "gui.h"
#include "prodriguezqu.h"
#include "util_CollisionManager.h"
#include "initGLX.h"
#include "player.h"
#include "world.h"
#include "initGLX.h"
#include "util_keys.h"
#include "keybinds.h"

GUI::GUI(World& world, Player& player) : world(world), player(player) 
{
	if (!numbers.init()) {}
	if (!letters.init()) {}
}


void GUI::render(float deltaTime) 
{
	static bool animation_complete = false; 
	if ((current_level_completed && !animation_complete)) {
		if(on_level_complete(deltaTime))
			animation_complete = true;
	}
	if (!current_level_completed && animation_complete)
		animation_complete = false;
	
	
	static bool close = false;
	static int DMkeyHoldCounter = 0; //DM - DevMode
	const int DMkeyHoldThreshold = 30; 

	if (handleKeyAction(keysPressed[stat_key] && (keysPressed[l_control_key] || keysPressed[r_control_key]), DMkeyHoldCounter, DMkeyHoldThreshold))
		close = !close;
	if (close)
		stats_screen(player, numbers, deltaTime);
	constant_stats(player, letters, numbers, deltaTime);
}

Shop::Shop(World& world, Player& player) : world(world), player(player), selectedIndex(0)
{
	if (!numbers.init()) {}
	if (!letters.init()) {}
}

void Shop::render(float deltaTime)
{
	std::string temp_str = "SHOP";
	letters.renderLetters(temp_str, 0, height - 150, 30);

	float itemSize = 140;
	float selectedSize = 160;
	float imageWidth = 600;
	static bool loaded = false;
	static Image boundingBox("images/rounded_square.png");
	static Image lockBox("images/lock.png");
	static Image crossBox("images/cross.png");
	if (!loaded) {
		boundingBox.loadTexture();
		lockBox.loadTexture();
		crossBox.loadTexture();
	}

   // Handle left/right key presses to change the selected accessory
	static float keyHoldCounter = 0;
	const float KeyHoldThreshold = 120;
    if (keysPressed[XK_Left] || keysPressed[XK_Right]) {
        if (keyHoldCounter <= 0) {
			if (keysPressed[XK_Left]) {
				selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : 0;
			}
			if (keysPressed[XK_Right]) {
				selectedIndex = (selectedIndex < collection.getAllAccessories().size() - 1) ? selectedIndex + 1 : selectedIndex;
			}
            keyHoldCounter = KeyHoldThreshold;
        } else {
            keyHoldCounter--;
        }
    } else {
        keyHoldCounter = 0;
    }

	const auto& accessories = collection.getAllAccessories(); // Now returns std::vector<std::unique_ptr<Accessory>>

	int centerPosition = width / 2; // Center position of the screen
	int startPosition = centerPosition - (width/2) - selectedIndex * imageWidth; // Start position for rendering

    for (size_t i = 0; i < accessories.size(); ++i) {
        //int xPosition = (-width/3) * 2 + i * imageWidth; // Assuming a fixed width for each image
	    int xPosition = startPosition + i * imageWidth; // Position each accessory

		// Skip rendering if the accessory is off-screen
		if (xPosition + imageWidth < 0 || xPosition - imageWidth > width)
			continue;

		// Dereference the unique_ptr to access the Accessory object
		const Accessory& accessory = *(accessories[i]);

		boundingBox.render(xPosition, height / 2, 180);
		float size = (i == selectedIndex) ? selectedSize : itemSize; // Larger size for selected item
		accessory.image->render(xPosition, height/2, size);
		// Render bounding box, name, description, and price
		if (accessory.bought)
			crossBox.render(xPosition, height / 2, 180);
		if (accessory.levelMin > levelsCompleted)
			lockBox.render(xPosition, height / 2, 180);
		letters.renderLetters(accessory.name, xPosition, height / 2 - 200, 30);
		letters.renderLetters(accessory.description, xPosition, height / 2 - 250, 20);
		numbers.renderNumbers(accessory.price, xPosition, height / 2 - 310, 30);


    }
	// if (keysPressed[enter_key]) {
	// 	const auto& selectedAccessory = collection.getAccessories()[selectedIndex];
	// 	if (player.getCoins() > selectedAccessory->price && !selectedAccessory->bought && !(selectedAccessory->levelMin > levelsCompleted)) {
	// 		printf("Adding Accessory, %s", selectedAccessory->name.c_str());
	// 		player.addAccessory(*selectedAccessory);
	// 		player.setCoins(player.getCoins() - selectedAccessory->price);
	// 		selectedAccessory->bought = true;
	// 	}
	// }

	if (keysPressed[enter_key]) {
		auto& selectedAccessory = collection.getAllAccessories()[selectedIndex];
		if (player.getCoins() > selectedAccessory->price && !selectedAccessory->bought && !(selectedAccessory->levelMin > levelsCompleted)) {
			printf("Adding Accessory, %s", selectedAccessory->name.c_str());
			// player.addAccessory(*selectedAccessory);
			player.addAccessory(selectedAccessory);
			player.setCoins(player.getCoins() - selectedAccessory->price);
			selectedAccessory->bought = true;
		}

	}
	loaded = true;
	player.updateStatwheel();

}

bool GUI::badge()
{

	return false;
}

Letters::Letters() {}

bool Letters::init() 
{
	if (!letters.loadTexture()) {
		std::cerr << "Failed to load texture" << std::endl;
		return false;
	}
	letters.setSpriteSheet(1,52);
	return true;
}

void Letters::renderLetters(const std::string& str, float x, float y, float scale) 
{
	if (str.empty()) {
		return; // Do nothing if the string is empty
	}

	float digitWidth = scale * 1.3; // Assuming each character has equal width
	float totalWidth = str.length() * digitWidth;
	float startX = x - totalWidth / 2; // Centering the string

	for (size_t i = 0; i < str.length(); ++i) {
		char c = str[i];
		int colIndex;

		if (c >= 'A' && c <= 'Z') {
			// Capital letters (A-Z) are in columns 0-25
			colIndex = c - 'A';
		} else if (c >= 'a' && c <= 'z') {
			// Lowercase letters (a-z) are in columns 26-51
			colIndex = 26 + (c - 'a');
		} else {
			// For characters outside A-Z and a-z, you might want to skip rendering or handle differently
			continue;
		}

		letters.renderSprite(0, colIndex, startX + i * digitWidth, y, scale);
	}
}

Numbers::Numbers() {}

bool Numbers::init() 
{
	if (!numbers.loadTexture()) {
		std::cerr << "Failed to load texture" << std::endl;
		return false;
	}
	numbers.setSpriteSheet(1,10);
	return true;
}

void Numbers::renderNumbers(float num, float x, float y, float scale) 
{
	if (num == 0.0f) {
		numbers.renderSprite(0, 0, x, y, scale); // Render '0' if the number is zero
		return;
	}

	// Extract integer part and first two digits after the decimal point
	int integerPart = static_cast<int>(num);
	int decimalPart = static_cast<int>(num * 100) % 100; 

	// Convert to string, add a period between the integer and decimal parts
	std::string numStr = std::to_string(integerPart) + "." + 
						 (decimalPart < 10 ? "0" : "") + std::to_string(decimalPart);

	float digitWidth = scale; // Assuming each digit (including decimal point) has equal width
	float totalWidth = (numStr.length() - 1) * digitWidth; // Subtract one for the decimal point
	float startX = x - totalWidth / 2; // Centering the number string

	for (size_t i = 0; i < numStr.length(); ++i) {
		char c = numStr[i];
		if (c == '.') {
			startX += digitWidth/32; // Move to the right for spacing of decimal point
			continue;
		}
		int digit = c - '0'; // Convert character to digit
		numbers.renderSprite(0, digit, startX + i * digitWidth, y, scale);
	}
}

void Numbers::renderInts(int num, float x, float y, float scale) 
{
	if (num == 0) {
		numbers.renderSprite(0, 0, x, y, scale); // Render '0' if the number is zero
		return;
	}

	// Convert number to string for easier digit handling
	std::string numStr = std::to_string(num);

	float digitWidth = scale * 1.3;
	float totalWidth = numStr.length() * digitWidth;
	float startX = x - totalWidth / 2; // Center the entire number string

	for (size_t i = 0; i < numStr.length(); ++i) {
		int digit = numStr[i] - '0'; // Convert character to digit
		numbers.renderSprite(0, digit, startX + i * digitWidth, y, scale);
	}
}
