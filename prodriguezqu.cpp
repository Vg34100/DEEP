#include "prodriguezqu.h"
/**
 * Author: Pablo Rodriguez
 * Created: 9.29.23 
*/ 

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




void showcaseHealth(int maxHealth, int currentHealth) {
	// Ensure that currentHealth does not exceed maxHealth
	if (currentHealth > maxHealth) {
		currentHealth = maxHealth;
	}

	// Screen dimensions, for example 800x600
	int screenWidth = width;
	int screenHeight = height;

	// Dimensions of the health bar
	int barWidth = 200; // for example
	int barHeight = 40; // for example

	// Choose a corner for the health bar, e.g., top-left
	int xPosition = screenWidth - barWidth - 10; // 10 pixels from the left
	int yPosition = screenHeight - barHeight - 10; // 10 pixels from the top

	// Calculate the width of the current health bar in relation to max health
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


enum class ScreenState {
	DELVE,
	OPTION,
	QUIT,
	MAX_STATES  // Helper to keep track of the number of states
};
// Function to handle state change
ScreenState changeState(ScreenState current, int direction) {
	// Convert enum class to int, adjust the state, and handle wrapping
	int newState = (static_cast<int>(current) + direction) % static_cast<int>(ScreenState::MAX_STATES);
	
	// If negative, wrap around to the last state
	if(newState < 0) newState = static_cast<int>(ScreenState::QUIT);
	
	// Convert back to ScreenState and return
	return static_cast<ScreenState>(newState);
}

bool rendering = false;
bool menu = true;
Image title("images/Deep.png");
Image start("images/Delve.png");
Image option("images/Options.png");
Image quit("images/Quit.png");
Image background("images/Hole.gif");
Image arrow("images/select.png");
ScreenState currentState = ScreenState::DELVE;
int titleScreen() {
	static int inputDelayCounter = 30; // Adjust as necessary
	if (!menu) {
		inputDelayCounter = 30;
		menu = true;
	}

	static int keyHoldCounter = 0;
	const int keyHoldThreshold = 30; // Example value, adjust as needed

	if (keysPressed[XK_Up] || keysPressed[XK_Down]) {
		if (keyHoldCounter <= 0) {
			int direction = keysPressed[XK_Up] ? -1 : 1;
			currentState = changeState(currentState, direction);
			keyHoldCounter = keyHoldThreshold;  // Reset the counter when state changes
		} else {
			keyHoldCounter--;  // Decrease the counter if key is held
		}
	} else {
		keyHoldCounter = 0;  // Reset the counter if key is not pressed
	}


	if(!rendering) {
		if (!title.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!start.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!option.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!quit.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!background.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!arrow.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		rendering = true;
	}
	background.render(0,0,height+10);
	title.render(0, height - 300, 150);
	start.render(0,50,30);
	option.render(0,-75,30);
	quit.render(0,-200,30);

	// static int keyHoldCounter = 0;
	// const int keyHoldThreshold = 30; // Example value, adjust as needed

	// If the delay counter hasn't reached 0, decrement it and skip input processing
	if (inputDelayCounter > 0) {
		inputDelayCounter--;
		return 0;
	}

	
	switch (currentState) {
		case ScreenState::DELVE:
			arrow.render(-200,50,30);
			if(keysPressed[XK_Return]) {
				menu = false;
				return 1;
			}
			break;
		case ScreenState::OPTION:
			arrow.render(-200,-75,30);
			if(keysPressed[XK_Return]) {
				menu = false;
				return 2;
			}
			break;
		case ScreenState::QUIT:
			arrow.render(-200,-200,30);
			if(keysPressed[XK_Return]) {
				return -1;
			}
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
	MAX_STATES  // Helper to keep track of the number of states
};
// Function to handle state change
OptionState changeState(OptionState current, int direction) {
	// Convert enum class to int, adjust the state, and handle wrapping
	int newState = (static_cast<int>(current) + direction) % static_cast<int>(OptionState::MAX_STATES);
	
	// If negative, wrap around to the last state
	if(newState < 0) newState = static_cast<int>(OptionState::QUIT);
	
	// Convert back to ScreenState and return
	return static_cast<OptionState>(newState);
}

bool options = false;
Image setMenu("images/setMenu.jpg");
bool settinginit = false;
OptionState optionState = OptionState::FULLSCREEN;
int optionScreen()
{
	if(!settinginit) {
		optionState = OptionState::FULLSCREEN;
		settinginit = true;
	}

	if(keysPressed[XK_Escape]) {
		settinginit = false;
		return -1;
	}


	static int keyHoldCounter = 0;
	const int keyHoldThreshold = 30; // Example value, adjust as needed

	if (keysPressed[XK_Up] || keysPressed[XK_Down]) {
		if (keyHoldCounter <= 0) {
			int direction = keysPressed[XK_Up] ? -1 : 1;
			optionState = changeState(optionState, direction);
			keyHoldCounter = keyHoldThreshold;  // Reset the counter when state changes
		} else {
			keyHoldCounter--;  // Decrease the counter if key is held
		}
	} else {
		keyHoldCounter = 0;  // Reset the counter if key is not pressed
	}



	if(!options) {
		if (!setMenu.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!option.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		if (!quit.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
		}
		if (!arrow.loadTexture()) {
			std::cerr << "Failed to load texture" << std::endl;
			//return -1;  // or handle error in some other way
		}
		options = true;
	}
	setMenu.render(0,0,height + 300);
	quit.render(-width+200,height-200,30);
	option.render(0,height-200,60);


	Rect r;
	//
	r.bot = height - 600;
	r.left = 10;
	r.center = 0;
	if(isFullscreen)
		ggprint16(&r, 16, 0x00ff0000, "FULLSCREEN");
	else {
		ggprint16(&r, 16, 0x00ff0000, "WINDOWED");
	}


	Rect h;
	//
	h.bot = height - 1000;
	h.left = 10;
	h.center = 0;
	std::string currentSize = std::to_string(static_cast<int>(width)) + " x " + std::to_string(static_cast<int>(height));
	ggprint16(&h, 16, 0x00ff0000, currentSize.c_str());





	switch(optionState) {
		case OptionState::QUIT: {
			arrow.render(-width+100,height-200,30);
			if(keysPressed[XK_Return]) {
			settinginit = false;
			return -1;
			}
			break;
		}
		case OptionState::FULLSCREEN: {
			arrow.render(-200,height-600,30);
			static int FSkeyHoldCounter = 0;
			const int FSkeyHoldThreshold = 30; // Example value, adjust as needed
			if (keysPressed[XK_Left] || keysPressed[XK_Right]) {

				if (FSkeyHoldCounter <= 0) {
					toggleFullscreen();
					FSkeyHoldCounter = FSkeyHoldThreshold;  // Reset the counter when state changes
				} else {
					FSkeyHoldCounter--;  // Decrease the counter if key is held
				}
			} else {
				FSkeyHoldCounter = 0;  // Reset the counter if key is not pressed
			}
			break;
		}
		case OptionState::SCREENSIZE: {
			arrow.render(-200,height-1000,30);
			static int SSkeyHoldCounter = 0;
			const int SSkeyHoldThreshold = 30; // Example value, adjust as needed

			if (keysPressed[XK_Left] || keysPressed[XK_Right]) {
				if (SSkeyHoldCounter <= 0) {
					bool direction = keysPressed[XK_Left] ? true : false;
					changeScreenSize(direction);
					SSkeyHoldCounter = SSkeyHoldThreshold;  // Reset the counter when state changes
				} else {
					SSkeyHoldCounter--;  // Decrease the counter if key is held
				}
			} else {
				SSkeyHoldCounter = 0;  // Reset the counter if key is not pressed
			}
			break;
		}
		default:
			break;
	}
	return 0;

} 


void levelenemyText() 
{
	Rect h;
	//
	h.bot = height - 50;
	h.left = -width + 50;
	h.center = 0;
	std::string levelsText = "Levels Completed: " + std::to_string(levelsCompleted);
	ggprint16(&h, 16, 0x00ff0000, levelsText.c_str());

	Rect e;
	//
	e.bot = height - 70;
	e.left = -width + 50;
	e.center = 0;
	std::string remainingText = "Enemies Remaining: " + std::to_string(enemiesRemaining);
	ggprint16(&e, 16, 0x00ff0000, remainingText.c_str());

	Rect t;
	//
	t.bot = height - 90;
	t.left = -width + 50;
	t.center = 0;
	std::string defeatedText = "Enemies Defeated: " + std::to_string(enemiesDefeated);
	ggprint16(&t, 16, 0x00ff0000, defeatedText.c_str());
}