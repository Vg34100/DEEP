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

void showcaseHealth(int maxHealth, int currentHealth) 
{
	// Screen dimensions, for example 800x600
	int screenWidth = width;
	int screenHeight = height;

	// Dimensions of the health bar
	int barWidth = 200; // for example
	int barHeight = 40; // for example

	// Choose a corner for the health bar, e.g., top-left
	int xPosition = screenWidth - barWidth - 10; 
	int yPosition = screenHeight - barHeight - 10; 

	// Ensure that currentHealth does not exceed maxHealth
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;

	// Calculate the width of the health bar in relation to max health
	int currentBarWidth = (currentHealth * barWidth) / maxHealth;

	// Draw the full health bar
	glColor3f(0.1, 0.1, 0.1); // Red color
	glBegin(GL_QUADS);
	glVertex2i(xPosition, yPosition);
	glVertex2i(xPosition + barWidth, yPosition);
	glVertex2i(xPosition + barWidth, yPosition + barHeight);
	glVertex2i(xPosition, yPosition + barHeight);
	glEnd();

	// Draw the current health bar above the full health bar
	glColor3f(0.0, 1.0, 0.0); // Green color
	glBegin(GL_QUADS);
	glVertex2i(xPosition, yPosition);
	glVertex2i(xPosition + currentBarWidth, yPosition);
	glVertex2i(xPosition + currentBarWidth, yPosition + barHeight);
	glVertex2i(xPosition, yPosition + barHeight);
	glEnd();
}

enum class ScreenState 
{
	DELVE,
	OPTION,
	QUIT,
	MAX_STATES  // Helper to keep track of the number of states
};

// Function to handle state change
ScreenState changeState(ScreenState current, int direction) 
{
	// Convert enum class to int, adjust the state, and handle wrapping
	int newState = (static_cast<int>(current) + direction) % 
	static_cast<int>(ScreenState::MAX_STATES);
	
	// If negative, wrap around to the last state
	if (newState < 0) 
		newState = static_cast<int>(ScreenState::QUIT);
	
	// Convert back to ScreenState and return
	return static_cast<ScreenState>(newState);
}

int titleScreen() 
{
	static Image title("images/Deep.png");
	static Image start("images/Delve.png");
	static Image option("images/Options.png");
	static Image quit("images/Quit.png");
	static Image arrow("images/Select.png");
	static Image background("images/Hole.gif");
	static ScreenState currentState = ScreenState::DELVE;
	static bool rendering = false;
	static bool menu = true;
	static int inputDelayCounter = 30; // Adjust as necessary
	static int keyHoldCounter = 0;
	const int keyHoldThreshold = 120; // Example value, adjust as needed

	if (!menu) {
		inputDelayCounter = 30;
		menu = true;
	}

	if (keysPressed[menu_up] || keysPressed[menu_down]) {
		if (keyHoldCounter <= 0) {
			int direction = keysPressed[menu_up] ? -1 : 1;
			currentState = changeState(currentState, direction);
			keyHoldCounter = keyHoldThreshold;  // Reset the counter
		} else
			keyHoldCounter--;  // Decrease the counter if key is held
	} else
		keyHoldCounter = 0;  // Reset the counter if key is not pressed

	if (!rendering) {
		if (!title.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!start.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!option.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!quit.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!background.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!arrow.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		rendering = true;
	}
	background.render(0,0,height+10);
	title.render(0, height - 300, 150);
	start.render(0,50,30);
	option.render(0,-75,30);
	quit.render(0,-200,30);

	// If the delay counter hasn't reached 0
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
		if (keysPressed[enter_key])
			return -1;
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
	MAX_STATES  // Helper to keep track of the number of states
};

enum class InputState {
	NORMAL,
	WAITING_FOR_INPUT
};

// Function to handle state change
OptionState changeState(OptionState current, int direction) {
	// Convert enum class to int, adjust the state, and handle wrapping
	int newState = (static_cast<int>(current) + direction) 
	% static_cast<int>(OptionState::MAX_STATES);
	
	// If negative, wrap around to the last state
	if (newState < 0) 
		newState = static_cast<int>(OptionState::QUIT);
	
	// Convert back to ScreenState and return
	return static_cast<OptionState>(newState);
}

int optionScreen()
{
	static Image setMenu("images/setMenu.jpg");
	static Image option("images/Options.png");
	static Image quit("images/Quit.png");
	static Image arrow("images/Select.png");

	static bool options = false;
	static bool settinginit = false;
	static OptionState optionState = OptionState::FULLSCREEN;
	static InputState inputState = InputState::NORMAL;
	static int keyHoldCounter = 0;
	const int keyHoldThreshold = 120; 

	static Letters letters;
	static Numbers numbers;

	if (!settinginit) {
		optionState = OptionState::FULLSCREEN;
		settinginit = true;
	}

	if (keysPressed[escape_key] && inputState != InputState::WAITING_FOR_INPUT) {
		settinginit = false;
		return -1;
	}

	if ((keysPressed[menu_up] || keysPressed[menu_down]) && inputState != InputState::WAITING_FOR_INPUT) {
		if (keyHoldCounter <= 0) {
			int direction = keysPressed[menu_up] ? -1 : 1;
			optionState = changeState(optionState, direction);
			keyHoldCounter = keyHoldThreshold;  // Reset the counter
		} else
			keyHoldCounter--;  // Decrease the counter if key is held
	} else
		keyHoldCounter = 0;  // Reset the counter if key is not pressed

	if (!options) {
		if (!setMenu.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!option.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!quit.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		if (!arrow.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		letters.init();
		numbers.init();
		options = true;
	}
	setMenu.render(0,0,height + 300);
	quit.render(-width+200,height-200,30);
	option.render(0,height-200,height*0.1);


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
		const int FSkeyHoldThreshold = 30; 
		if (keysPressed[menu_left] || keysPressed[menu_right]) {
			if (FSkeyHoldCounter <= 0) {
				toggleFullscreen();
				FSkeyHoldCounter = FSkeyHoldThreshold;  
			} else
				FSkeyHoldCounter--;  
		} else
			FSkeyHoldCounter = 0;  
		break;
	}
	case OptionState::SCREENSIZE: {
		arrow.render(-600,height-500,30);
		static int SSkeyHoldCounter = 0;
		const int SSkeyHoldThreshold = 30; 

		if (keysPressed[menu_left] || keysPressed[menu_right]) {
			if (SSkeyHoldCounter <= 0) {
				bool direction = keysPressed[menu_left] ? true : false;
				changeScreenSize(direction);
				SSkeyHoldCounter = SSkeyHoldThreshold;  
			} else
				SSkeyHoldCounter--;  
		} else
			SSkeyHoldCounter = 0; 
		break;
	}
	case OptionState::ATTACK_KEY: {
		arrow.render(-600,height-650,30);
		static int temp_key = attack_key;
		if (optionState == OptionState::ATTACK_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			attack_key = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			attack_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				attack_key = key;
				inputState = InputState::NORMAL;
			} else {
				attack_key = temp_key;
			}
		} else {
			attack_input_size = 30;
		}
		break;
	}
	case OptionState::MOVE_UP_KEY: {
		arrow.render(-600,height-700,30);
		static int temp_key = move_up;
		if (optionState == OptionState::MOVE_UP_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			move_up = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			move_up_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				move_up = key;
				inputState = InputState::NORMAL;
			} else {
				move_up = temp_key;
			}
		} else {
			move_up_input_size = 30;
		}
		break;		
	}
	case OptionState::MOVE_DOWN_KEY: {
		arrow.render(-600,height-750,30);
		static int temp_key = move_down;
		if (optionState == OptionState::MOVE_DOWN_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			move_down = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			move_down_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				move_down = key;
				inputState = InputState::NORMAL;
			} else {
				move_down = temp_key;
			}
		} else {
			move_down_input_size = 30;
		}
		break;		
	}
	case OptionState::MOVE_LEFT_KEY: {
		arrow.render(-600,height-800,30);
		static int temp_key = move_left;
		if (optionState == OptionState::MOVE_LEFT_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			move_left = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			move_left_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				move_left = key;
				inputState = InputState::NORMAL;
			} else {
				move_left = temp_key;
			}
		} else {
			move_left_input_size = 30;
		}
		break;		
	}
	case OptionState::MOVE_RIGHT_KEY: {
		arrow.render(-600,height-850,30);
		static int temp_key = move_right;
		if (optionState == OptionState::MOVE_RIGHT_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			move_right = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			move_right_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				move_right = key;
				inputState = InputState::NORMAL;
			} else {
				move_right = temp_key;
			}
		} else {
			move_right_input_size = 30;
		}
		break;		
	}
	case OptionState::RELOAD_KEY: {
		arrow.render(-600,height-900,30);
		static int temp_key = reload_key;
		if (optionState == OptionState::RELOAD_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			reload_key = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			reload_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				reload_key = key;
				inputState = InputState::NORMAL;
			} else {
				reload_key = temp_key;
			}
		} else {
			reload_input_size = 30;
		}
		break;		
	}
	case OptionState::DODGE_KEY: {
		arrow.render(-600,height-950,30);
		static int temp_key = dodge_key;
		if (optionState == OptionState::DODGE_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			dodge_key = XK_Clear;
		}
		if (inputState == InputState::WAITING_FOR_INPUT) {
			dodge_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				dodge_key = key;
				inputState = InputState::NORMAL;
			} else {
				dodge_key = temp_key;
			}
		} else {
			dodge_input_size = 30;
		}
		break;		
	}
	case OptionState::SWITCH_WEAPON_L_KEY: {
		arrow.render(-600,height-1000,30);
		static int temp_key = switch_weapon_left;
		if (optionState == OptionState::SWITCH_WEAPON_L_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			switch_weapon_left = XK_Clear;
		}
	   if (inputState == InputState::WAITING_FOR_INPUT) {
			switch_weapon_l_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				switch_weapon_left = key;
				inputState = InputState::NORMAL;
			} else {
				switch_weapon_left = temp_key;
			}
		} else {
			switch_weapon_l_input_size = 30;
		}
		break;		
	}
	case OptionState::SWITCH_WEAPON_R_KEY: {
		arrow.render(-600,height-1050,30);
		static int temp_key = switch_weapon_right;
		if (optionState == OptionState::SWITCH_WEAPON_R_KEY && key == XK_Return) {
			inputState = InputState::WAITING_FOR_INPUT;
			switch_weapon_right = XK_Clear;
		}
	   if (inputState == InputState::WAITING_FOR_INPUT) {
			switch_weapon_r_input_size = 40;
			if (key != XK_Return && key != XK_Escape && key != XK_Shift_L && key != XK_Control_L && key != menu_up && key != menu_down/*&&  ... other keys to exclude ... */) {
				switch_weapon_right = key;
				inputState = InputState::NORMAL;
			} else {
				switch_weapon_right = temp_key;
			}
		} else {
			switch_weapon_r_input_size = 30;
		}
		break;		
	}
	default:
		break;
	}



	/* Display Fullscreen Setting */
	if (isFullscreen) {
		//ggprint16(&r, 16, 0x00ff0000, "FULLSCREEN");
		std::string str = "FULLSCREEN";
		letters.renderLetters(str, 0, height-400, 50);

	}
	else {
		//ggprint16(&r, 16, 0x00ff0000, "WINDOWED");
		std::string str = "WINDOWED";
		letters.renderLetters(str, 0, height-400, 50);
	}


	/* Display Screen Size */
	std::string currentSize = "x";
	letters.renderLetters(currentSize, 0, height-500, 30);
	numbers.renderInts(actual_width,-150,height-500,30);
	numbers.renderInts(actual_height,150,height-500,30);


	/* Display Attack Keybind */
	std::string temp_str;
	temp_str = std::string("Attack: ") + XKeysymToString(attack_key);
	letters.renderLetters(temp_str, 0,height-650,attack_input_size);

	temp_str = std::string("Move Up: ") + XKeysymToString(move_up);
	letters.renderLetters(temp_str, 0,height-700,move_up_input_size);

	temp_str = std::string("Move Down: ") + XKeysymToString(move_down);
	letters.renderLetters(temp_str, 0,height-750,move_down_input_size);

	temp_str = std::string("Move Left: ") + XKeysymToString(move_left);
	letters.renderLetters(temp_str, 0,height-800,move_left_input_size);

	temp_str = std::string("Move Right: ") + XKeysymToString(move_right);
	letters.renderLetters(temp_str, 0,height-850,move_right_input_size);



	temp_str = std::string("Reload: ") + XKeysymToString(reload_key);
	letters.renderLetters(temp_str, 0,height-900,reload_input_size);

	temp_str = std::string("Dodge: ") + XKeysymToString(dodge_key);
	letters.renderLetters(temp_str, 0,height-950,dodge_input_size);

	temp_str = std::string("Switch Left: ") + XKeysymToString(switch_weapon_left);
	letters.renderLetters(temp_str, 0,height-1000,switch_weapon_l_input_size);

	temp_str = std::string("Switch Right: ") + XKeysymToString(switch_weapon_right);
	letters.renderLetters(temp_str, 0,height-1050,switch_weapon_r_input_size);

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

	Animation() : startY(0), endY(0), duration(1), elapsedTime(0), state(AnimationState::Idle) {}

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
			if (elapsedTime >= waiting_duration) { // Wait for 2 seconds at the top
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
		if (state == AnimationState::Idle || state == AnimationState::WaitingAtTop)
			return endY;
		float t = elapsedTime / duration;
		return (state == AnimationState::MovingUp) ? (startY + t * (endY - startY)) : (endY + t * (startY - endY));
	}
};



bool on_level_complete(float deltaTime) {
	static Animation clearAnimation;
	static bool options = false;
	static Image clear("images/all_defeated.png");

	if (!options) {
		if (!clear.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		clearAnimation.start(-height - 200, -height/2 - 200, 0.4, 2.2); // Start animation from bottom to middle
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

void on_level_enter()
{

}

Animation statsAnimation;

bool stats_screen(Player& player, Numbers& numbers, float deltaTime) {
	static bool loaded = false;
	static Image stats("images/stats_icons.png");
	if (!loaded) {
		if (!stats.loadTexture())
			std::cerr << "Failed to load texture" << std::endl;
		stats.setSpriteSheet(1,11);
		loaded = true;
	}
	stats.renderSprite(0,0,-width + 80,height - 200,40); // Health
	stats.renderSprite(0,1,-width + 80,height - 200 - 100 * 1,40); // Mana
	stats.renderSprite(0,2,-width + 80,height - 200 - 100 * 2,40); // Ammo
	stats.renderSprite(0,3,-width + 80,height - 200- 100 * 3,40); // Sanity
	stats.renderSprite(0,4,-width + 80,height - 200- 100 * 4,40); // Speed
	stats.renderSprite(0,5,-width + 80,height - 200- 100 * 5,40); // Damage
	stats.renderSprite(0,6,-width + 80,height - 200- 100 * 6,40); // Resistance
	stats.renderSprite(0,7,-width + 80,height - 200- 100 * 7,40); // AttackSpeed
	stats.renderSprite(0,8,-width + 80,height - 200- 100 * 8,40); // Range
	stats.renderSprite(0,9,-width + 80,height - 200- 100 * 9,40); // Luck
	stats.renderSprite(0,10,-width + 80,height - 200- 100 * 10,40); // Size

	numbers.renderNumbers(player.getStatwheel(0),-width + 120,height - 200 - 100 * 0,15);	
	numbers.renderNumbers(player.getStatwheel(1),-width + 120,height - 200 - 100 * 1,15);	
	numbers.renderNumbers(player.getStatwheel(2),-width + 120,height - 200 - 100 * 2,15);	
	numbers.renderNumbers(player.getStatwheel(3),-width + 120,height - 200 - 100 * 3,15);	
	numbers.renderNumbers(player.getStatwheel(4),-width + 120,height - 200 - 100 * 4,15);	
	numbers.renderNumbers(player.getStatwheel(5),-width + 120,height - 200 - 100 * 5,15);	
	numbers.renderNumbers(player.getStatwheel(6),-width + 120,height - 200 - 100 * 6,15);	
	numbers.renderNumbers(player.getStatwheel(7),-width + 120,height - 200 - 100 * 7,15);	
	numbers.renderNumbers(player.getStatwheel(8),-width + 120,height - 200 - 100 * 8,15);	
	numbers.renderNumbers(player.getStatwheel(9),-width + 120,height - 200 - 100 * 9,15);	
	numbers.renderNumbers(player.getStatwheel(10),-width + 120,height - 200 - 100 * 10,15);	


	return true;
}

enum class PausedState {
	CONTINUE,
	SETTINGS,
	QUIT,
	MAX_STATES
};

PausedState changeState(PausedState current, int direction) {
	// Convert enum class to int, adjust the state, and handle wrapping
	int newState = (static_cast<int>(current) + direction) 
	% static_cast<int>(PausedState::MAX_STATES);
	
	// If negative, wrap around to the last state
	if (newState < 0) 
		newState = static_cast<int>(PausedState::CONTINUE);
	
	// Convert back to ScreenState and return
	return static_cast<PausedState>(newState);
}

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
	const int keyHoldThreshold = 120; 
	if ((keysPressed[menu_up] || keysPressed[menu_down])) {
		if (keyHoldCounter <= 0) {
			int direction = keysPressed[menu_up] ? -1 : 1;
			paustedState = changeState(paustedState, direction);
			keyHoldCounter = keyHoldThreshold;  // Reset the counter
		} else
			keyHoldCounter--;  // Decrease the counter if key is held
	} else
		keyHoldCounter = 0;  // Reset the counter if key is not pressed

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