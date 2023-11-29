/*
 * Author: Pablo Rodriguez
 * Created: 9.29.23 
*/ 
#include "prodriguezqu.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <iostream>
#include "initGLX.h"
#include "image.h"
#include "fonts.h"
#include "world.h"
#include "gui.h"
#include "player.h"
#include "keybinds.h"
#include <string>
#include "util_vector2.h"

typedef struct {
    float red;
    float green;
    float blue;
} Color;

template<typename T>
void clamp(T& value, const T& minVal, const T& maxVal) 
{
    value = std::max(minVal, std::min(value, maxVal));
}

//0123456789012345678901234567890123456789012345678901234567890123456789012345
void drawBar(Vector2 pos, Vector2 dim, Color color, bool center = false) 
{
	/* Function to Draw Bar */
	/* Position, Dimensions [Width, Height], Color [Red, Green, Blue]*/
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);

	if (!center) {
		glVertex2i(pos.x, pos.y);
		glVertex2i(pos.x + dim.x, pos.y);
		glVertex2i(pos.x + dim.x, pos.y + dim.y);
		glVertex2i(pos.x, pos.y + dim.y);
	} else {
		float halfWidth = dim.x / 2;
		float halfHeight = dim.y / 2;
		glVertex2i(pos.x - halfWidth, pos.y - halfHeight);
		glVertex2i(pos.x + halfWidth, pos.y - halfHeight);
		glVertex2i(pos.x + halfWidth, pos.y + halfHeight);
		glVertex2i(pos.x - halfWidth, pos.y + halfHeight);
	}
	glEnd();
}

//0123456789012345678901234567890123456789012345678901234567890123456789012345
void renderHealthBar(int maxHealth, int currentHealth) 
{
    /* Configure Health Bar Appearance */
    Vector2 barSize = {200.0f, 40.0f}; // Health bar dimensions
    Vector2 barPosition = {
        width - barSize.x - 10.0f, // 10px from the right edge
        height - barSize.y - 10.0f // 10px from the bottom edge
    };

    /* Ensure Current Health is Within Range */
    clamp(currentHealth, 0, maxHealth);

    /* Calculate Health Bar Width */
    float healthRatio = static_cast<float>(currentHealth) / maxHealth;
    float currentWidth = healthRatio * barSize.x;

    /* Define Colors for Health Bar States */
    Color maxHealthColor = {0.1, 0.1, 0.1}; // Color for full health
    Color currentHealthColor = {0.0, 1.0, 0.0}; // Color for current health

    /* Draw the Full Health Bar */
    drawBar(barPosition, barSize, maxHealthColor);

    /* Draw the Current Health Bar */
    Vector2 currentBarSize = {currentWidth, barSize.y};
    drawBar(barPosition, currentBarSize, currentHealthColor);
}


enum class ScreenState 
{
	DELVE,
	OPTION,
	QUIT,
	MAX_STATES
};




//012345678901234567890123456789012345678901234567890123456789012345678901234567
const int InputDelayReset = 30;
const int KeyHoldThreshold = 60;

void processMenuInput(
    bool inputCondition, 
    int& keyHoldCounter, 
    std::function<void()> actionFunction) 
{
    if (inputCondition) {
        if (keyHoldCounter <= 0) {
            actionFunction(); // Perform the passed action
            keyHoldCounter = KeyHoldThreshold;
        } else {
            keyHoldCounter--;
        }
    } else {
        keyHoldCounter = 0;
    }
}


int titleScreen() 
{
    /* Static variables for menu state and images */
    static Image title("images/Deep.png"), start("images/Delve.png"),
                 option("images/Options.png"), quit("images/Quit.png"),
                 arrow("images/Select.png"), background("images/Hole.gif");
	static ScreenState currentState = ScreenState::DELVE;
    static bool menu = true;
    static int inputDelayCounter = InputDelayReset, keyHoldCounter = 0;

	if (!menu) {
		inputDelayCounter = InputDelayReset;
		menu = true;
	}

	bool menuMoved = keysPressed[menu_up] || keysPressed[menu_down];
	processMenuInput(menuMoved, keyHoldCounter, [&](){
		changeState(currentState, keysPressed[menu_up] ? -1 : 1);
	});

    // Load textures once
//0123456789012345678901234567890123456789012345678901234567890123456789012345

	background.render(0,0,height+10);
	title.render(0, height - 300, 150);
	start.render(0,50,30);
	option.render(0,-75,30);
	quit.render(0,-200,30);

    // Input delay handling
	if (inputDelayCounter > 0) {
		inputDelayCounter--;
		return 0;
	}

	switch (currentState) {
	case ScreenState::DELVE:
		arrow.render(-200,50,30);
		if (keysPressed[enter_key]) {
			menu = false;
			return 1;
		}
		break;
	case ScreenState::OPTION:
		arrow.render(-200,-75,30);
		if (keysPressed[enter_key]) {
			menu = false;
			return 2;
		}
		break;
	case ScreenState::QUIT:
		arrow.render(-200,-200,30);
		if (keysPressed[enter_key]) return -1;
		break;
	default:
		break;
	}
	return 0;
}

enum class OptionState {
	QUIT,
	FULLSCREEN,
	SCREENSIZE,
	ATTACK_KEY,
	MOVE_UP_KEY,
	MOVE_DOWN_KEY,
	MOVE_LEFT_KEY,
	MOVE_RIGHT_KEY,
	RELOAD_KEY,
	DODGE_KEY,
	SWITCH_WEAPON_L_KEY,
	SWITCH_WEAPON_R_KEY,
	MAX_STATES 
};

enum class InputState {
	NORMAL,
	WAITING_FOR_INPUT
};

//0123456789012345678901234567890123456789012345678901234567890123456789012345
void handleOptionKey(bool stateCheck,
                     int& keyVar, int& size, int key, InputState& inputState) {
	static int tempKey = keyVar;
    if (stateCheck && key == XK_Return) {
        inputState = InputState::WAITING_FOR_INPUT;
        keyVar = XK_Clear; // Clear the key variable
    }

    // Check if waiting for new input
    if (inputState == InputState::WAITING_FOR_INPUT) {
        size = 40; // Increase the input size
        if (key != XK_Return && key != XK_Escape &&
            key != XK_Shift_L && key != XK_Control_L && 
            key != menu_up && key != menu_down
            /*&&  ... other keys to exclude ... */) {
            keyVar = key;
            inputState = InputState::NORMAL;
        } else {
            keyVar = tempKey; // Revert to the original key 
        }
    } else {
        size = 30; // Default input size
    }
}

struct Keybind {
    std::string action;
    int key;
    int inputSize;
};

int optionScreen()
{
	/* Rendering & Logic of the Options Menu*/
	static Image setMenu("images/setMenu.jpg");
	static Image option("images/Options.png");
	static Image quit("images/Quit.png");
	static Image arrow("images/Select.png");

	static bool options = false;
	static bool settinginit = false;
	static OptionState optionState = OptionState::FULLSCREEN;
	static InputState inputState = InputState::NORMAL;
	static int keyHoldCounter = 0;

	static Letters letters;
	static Numbers numbers;

	if (!settinginit) {
		optionState = OptionState::FULLSCREEN;
		settinginit = true;
	}

	if (keysPressed[escape_key] && 
			inputState != InputState::WAITING_FOR_INPUT) {
		settinginit = false;
		return -1;
	}

	bool menuMoved = (keysPressed[menu_up] || keysPressed[menu_down]) &&
	 				 inputState != InputState::WAITING_FOR_INPUT;
	processMenuInput(menuMoved, keyHoldCounter, [&](){
		changeState(optionState, keysPressed[menu_up] ? -1 : 1);
	});

	if (!options) {
		options = true;
		letters.init();
		numbers.init();
	}
	setMenu.render(0,0,height + 300);
	quit.render(-width+200,height-200,30);
	option.render(0,height-200,height*0.1);

	/* GAMEPLAY KEYBINDINGS: */
	/*
	ATTACK_KEY
	MOVE_UP_KEY,
	MOVE_DOWN_KEY,
	MOVE_LEFT_KEY,
	MOVE_RIGHT_KEY,
	RELOAD_KEY,
	DODGE_KEY,
	SWITCH_WEAPON_L_KEY,
	SWITCH_WEAPON_R_KEY,
	*/

	int attack_input_size = 30;
	int move_up_input_size = 30;
	int move_down_input_size = 30;
	int move_left_input_size = 30;
	int move_right_input_size = 30;
	int reload_input_size = 30;
	int dodge_input_size = 30;
	int switch_weapon_l_input_size = 30;
	int switch_weapon_r_input_size = 30;

	switch(optionState) {
	case OptionState::QUIT: {
		arrow.render(-width+100,height-200,30);
		if (keysPressed[enter_key]) {
		settinginit = false;
		return -1;
		}
		break;
	}
	case OptionState::FULLSCREEN: {
		arrow.render(-600,height-400,30);
		static int FSkeyHoldCounter = 0;
		bool fullscreenToggle = keysPressed[menu_left] ||
									 keysPressed[menu_right];
		processMenuInput(fullscreenToggle, FSkeyHoldCounter, [&](){
			toggleFullscreen();
		});
		break;
	}
	case OptionState::SCREENSIZE: {
		arrow.render(-600,height-500,30);
		static int SSkeyHoldCounter = 0;
		bool screenSizeToggle = keysPressed[menu_left] || 
										keysPressed[menu_right];
		processMenuInput(screenSizeToggle, SSkeyHoldCounter, [&](){
			changeScreenSize(keysPressed[menu_left] ? true : false);
		});
		break;
	}
	case OptionState::ATTACK_KEY: {
		arrow.render(-600,height-650,30);
		bool keyCheck = optionState == OptionState::ATTACK_KEY;
        handleOptionKey(keyCheck, attack_key, attack_input_size,
												 key, inputState);
		break;
	}
	case OptionState::MOVE_UP_KEY: {
		arrow.render(-600,height-700,30);
		bool keyCheck = optionState == OptionState::MOVE_UP_KEY;
        handleOptionKey(keyCheck, move_up, move_up_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::MOVE_DOWN_KEY: {
		arrow.render(-600,height-750,30);
		bool keyCheck = optionState == OptionState::MOVE_DOWN_KEY;
        handleOptionKey(keyCheck, move_down, move_down_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::MOVE_LEFT_KEY: {
		arrow.render(-600,height-800,30);
		bool keyCheck = optionState == OptionState::MOVE_LEFT_KEY;
        handleOptionKey(keyCheck, move_left, move_left_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::MOVE_RIGHT_KEY: {
		arrow.render(-600,height-850,30);
		bool keyCheck = optionState == OptionState::MOVE_RIGHT_KEY;
        handleOptionKey(keyCheck, move_right, move_right_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::RELOAD_KEY: {
		arrow.render(-600,height-900,30);
		bool keyCheck = optionState == OptionState::RELOAD_KEY;
        handleOptionKey(keyCheck, reload_key, reload_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::DODGE_KEY: {
		arrow.render(-600,height-950,30);
		bool keyCheck = optionState == OptionState::DODGE_KEY;
        handleOptionKey(keyCheck, dodge_key, dodge_input_size,
												 key, inputState);
		break;		
	}
	case OptionState::SWITCH_WEAPON_L_KEY: {
		arrow.render(-600,height-1000,30);
		bool keyCheck = optionState == OptionState::SWITCH_WEAPON_L_KEY;
        handleOptionKey(keyCheck, switch_weapon_left, 
							switch_weapon_l_input_size, key, inputState);

		break;		
	}
	case OptionState::SWITCH_WEAPON_R_KEY: {
		arrow.render(-600,height-1050,30);
		bool keyCheck = optionState == OptionState::SWITCH_WEAPON_R_KEY;
        handleOptionKey(keyCheck, switch_weapon_right, 
							switch_weapon_r_input_size, key, inputState);

		break;		
	}
	default:
		break;
	}
//0123456789012345678901234567890123456789012345678901234567890123456789012345


	/* Display Fullscreen Setting */
	if (isFullscreen) {
		std::string str = "FULLSCREEN";
		letters.renderLetters(str, 0, height-400, 50);
	}
	else {
		std::string str = "WINDOWED";
		letters.renderLetters(str, 0, height-400, 50);
	}


	/* Display Screen Size */
	std::string currentSize = "x";
	letters.renderLetters(currentSize, 0, height-500, 30);
	numbers.renderInts(actual_width,-150,height-500,30);
	numbers.renderInts(actual_height,150,height-500,30);

	std::vector<Keybind> keybinds = {
		{"Attack", attack_key, attack_input_size},
		{"Move Up", move_up, move_up_input_size},
		{"Move Down", move_down, move_down_input_size},
		{"Move Left", move_left, move_left_input_size},
		{"Move Right", move_right, move_right_input_size},
		{"Reload", reload_key, reload_input_size},
		{"Dodge", dodge_key, dodge_input_size},
		{"Switch Left", switch_weapon_left, switch_weapon_l_input_size},
		{"Switch Right", switch_weapon_right, switch_weapon_r_input_size}
	};

	const int yOffset = 50; // Vertical offset between each keybind

	for (size_t i = 0; i < keybinds.size(); ++i) {
		const auto& keybind = keybinds[i];
		std::string temp_str = keybind.action + ": " + XKeysymToString(keybind.key);
		letters.renderLetters(temp_str, 0, height - (650 + i * yOffset), keybind.inputSize);
	}

	return 0;

} 

int running_time(int &since, const bool get)
{
	static int firsttime = 1;
	static int start_time;
	if (firsttime || !get) {
		start_time = time(NULL);
		firsttime=0;
		since = 0;
	}
	if (get)
		since = time(NULL) - start_time;
	return 0;
}

int mouse_since_counter(const bool reset, bool render)
{
	static int firsttime = 1;
	static int start_time;
	static int mouse_since = 0;
	if (firsttime || reset) {
		start_time = time(NULL);
		firsttime=0;
		mouse_since = 0;
	}
	if (!reset)
		mouse_since = time(NULL) - start_time;
	if (render)
		renderRect("Since Mouse: ", mouse_since);
	return 0;
}

bool firstload = true;
void renderRect(std::string text, int value) 
{
	static Rect stats;
	std::string levelsText = text + std::to_string(value);
	if (firstload) {
		stats.bot = height - 500;
		stats.left = -width + 50;
		stats.center = 0;
		firstload = !firstload;
	}
	ggprint16(&stats, 16, 0x00ff0000, levelsText.c_str());
}


void levelenemyText(double elapsedtime) 
{
	firstload = true;
	renderRect("Elapsed Time: ", elapsedtime);
	renderRect("Levels Completed: ", levelsCompleted);
	renderRect("Enemies Remaining: ", enemiesRemaining);
	renderRect("Enemies Defeated: ", enemiesDefeated);
	mouse_since_counter(false, true);
}


enum class AnimationState {
	Idle,
	MovingUp,
	WaitingAtTop,
	MovingDown
};

struct Animation {
	float startY;
	float endY;
	float duration;
	float waiting_duration;
	float elapsedTime;
	AnimationState state;
	bool animationInitiated;
	bool animationCompleted;
	Animation() : startY(0), endY(0), duration(1), 
						elapsedTime(0), state(AnimationState::Idle) {}

	void start(float start, float end, float dur, float waitdur) {
		startY = start;
		endY = end;
		duration = dur;
		waiting_duration = waitdur;
		elapsedTime = 0;
		state = AnimationState::MovingUp;
	}

	void update(float deltaTime) {
		if (state == AnimationState::Idle)
			return;

		elapsedTime += deltaTime;
		if (state == AnimationState::MovingUp) {
			if (elapsedTime >= duration) {
				elapsedTime = 0;
				state = AnimationState::WaitingAtTop;
			}
		} else if (state == AnimationState::WaitingAtTop) {
			if (elapsedTime >= waiting_duration) { 
				elapsedTime = 0;
				state = AnimationState::MovingDown;
			}
		} else if (state == AnimationState::MovingDown) {
			if (elapsedTime >= duration) {
				state = AnimationState::Idle;
			}
		}
	}

	float getCurrentY() const {
		if (state == AnimationState::Idle || 
				state == AnimationState::WaitingAtTop)
			return endY;
		float t = elapsedTime / duration;
		return (state == AnimationState::MovingUp) ? 
			(startY + t * (endY - startY)) : (endY + t * (startY - endY));
	}
};



bool on_level_complete(float deltaTime) 
{
	static Animation clearAnimation;
	static bool options = false;
	static Image clear("images/all_defeated.png");

	if (!options) {
		// if (!clear.loadTexture())
		// 	std::cerr << "Failed to load texture" << std::endl;
		clearAnimation.start(-height - 200, -height/2 - 200, 0.4, 2.2); 
		options = true;
		clearAnimation.animationInitiated = true;
		clearAnimation.animationCompleted = false;
	}

	clearAnimation.update(deltaTime);
	clear.render(0, clearAnimation.getCurrentY(), height * 0.13);

	if (clearAnimation.state == AnimationState::Idle) {
		options = false;
		return true;
	}
	return false;
}

Animation statsAnimation;

bool stats_screen(Player& player, Numbers& numbers, float deltaTime) 
{
	static bool loaded = false;
	static Image stats("images/stats_icons.png");
	if (!loaded) {
		// loaded = stats.loadTexture();
		stats.setSpriteSheet(1,11);
		loaded = true;
	}

	const int numStats = 11; // Total number of stats
	const int yOffset = 100; // Vertical offset between each stat
	const int xOffset = 80;  // Horizontal offset for sprites
	const int numberOffsetX = 120; // Horizontal offset for numbers
	const int initialY = height - 200; // Initial Y position
	const int spriteSize = 40; // Size for sprites
	const int numberSize = 15; // Size for numbers

	for (int i = 0; i < numStats; ++i) {
		int yPos = initialY - yOffset * i;
		stats.renderSprite(0, i, -width + xOffset, yPos, spriteSize); 
		numbers.renderNumbers(player.getStatwheel(i),
		 							-width + numberOffsetX, yPos, numberSize);
	}




	return true;
}

bool constant_stats(Player& player, Letters& letters, Numbers& numbers, float deltaTime) 
{
	static Image dollars("images/dollar.png");
	dollars.render(-width + 120, height - 40, 30);

	const int numberSize = 15; // Size for numbers
	numbers.renderNumbers(player.getCoins(), 
									-width + 150, height - 50, numberSize);

	static Image backpack("images/backpack.png");
	backpack.render(width - 100, -height + 100, 50);
	std::string temp_str = "P";
	letters.renderLetters(temp_str, width - 120, -height + 120, 20);

	static Image cart("images/cart.png");
	cart.render(width - 100, -height + 200, 50);
	temp_str = "L";
	letters.renderLetters(temp_str, width - 120, -height + 220, 20);


	return true;
}


enum class PausedState {
	CONTINUE,
	SETTINGS,
	QUIT,
	MAX_STATES
};

int paused()
{
	static Letters letters;
	static PausedState paustedState = PausedState::CONTINUE;
	static bool init = false;
	if (!init) {
		letters.init();
		init = true;
	}
	static int keyHoldCounter = 0;
	bool menuMoved = keysPressed[menu_up] || keysPressed[menu_down];
	processMenuInput(menuMoved, keyHoldCounter, [&](){
		changeState(paustedState, keysPressed[menu_up] ? -1 : 1);
	});

	int continue_size = 60;
	int settings_size = 60;
	int quit_size = 60;

	switch(paustedState) {
	case PausedState::CONTINUE: {
		continue_size = 80;
		if (keysPressed[enter_key]) 
			return -1;
		break;
	}
	case PausedState::SETTINGS: {
		settings_size = 80;
		if (keysPressed[enter_key]) 
			return 1;
		break;
	}
	case PausedState::QUIT: {
		quit_size = 80;
		if (keysPressed[enter_key]) 
			return 2;
		break;
	}
	default:
		break;
	}

	std::string temp_str;
	temp_str = "PAUSED";
	letters.renderLetters(temp_str,0,height-200,100);

	temp_str = "Continue";
	letters.renderLetters(temp_str,0,height-600,continue_size);
	temp_str = "Settings";
	letters.renderLetters(temp_str,0,height-750,settings_size);
	temp_str = "Quit";
	letters.renderLetters(temp_str,0,height-900,quit_size);

	return 0;
}
//0123456789012345678901234567890123456789012345678901234567890123456789012345