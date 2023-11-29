#include "apacheco2.h"
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
#include <functional>


/**
 * Author: Aaron Pacheco
 * Created: 9.29.23 
*/
using namespace std;

bool keyCheck = false;


/*int total_running_time(const bool get)
{
    static int firsttime = 1;
    static int start_time;
    if (firsttime) {
        start_time = time(NULL);
        firsttime = 0;
    }
    if (get) {
        return time(NULL) - start_time;
    }
    return 0;

} */
int time_since_key_press(const bool get) {
    
    static int firsttime = 1;
    static int start_time;
    static int delta = 0;

    renderRect("Time since key press", delta);
    //initializes first time
    if (firsttime || !get) { 
        start_time = time(NULL);
        delta = 0;
        firsttime = 0;
    }
    //resets time if key pressed
    if (get) {
    //    cout << "yey" << endl;
        delta = time(NULL) - start_time;
        return delta;
    }
    
   
    return 0;

}


//gameover
enum class gameOverState {
	RETRY,
	SETTINGS,
	QUIT,
	MAX_STATES
};




int gameOver()
{
	static Letters letters;
	static gameOverState game_over = gameOverState::RETRY;

	static bool init = false;

	if(!init){
		letters.init();
		init = true;
	}

	static int keyHoldCounter = 0;
	bool menuMoved = keysPressed[menu_up] || keysPressed[menu_down];
	processMenuInput(menuMoved, keyHoldCounter, [&](){
		changeState(game_over, keysPressed[menu_up] ? -1 : 1);
	});

	int retry_size = 60;
	int settings_size = 60;
	int quit_size = 60;

	switch(game_over) {
	case gameOverState::RETRY: {
		retry_size = 80;
		if (keysPressed[enter_key]) 
			return -1;
		break;
	}
	case gameOverState::SETTINGS: {
		settings_size = 80;
		if (keysPressed[enter_key]) 
			return 1;
		break;
	}
	case gameOverState::QUIT: {
		quit_size = 80;
		if (keysPressed[enter_key]) 
			return 2;
		break;
	}
	default:
		break;
	}

	std::string temp_str;
	temp_str = "GAME OVER";
	letters.renderLetters(temp_str,0,height-200,100);

	temp_str = "Retry";
	letters.renderLetters(temp_str,0,height-600,retry_size);
	temp_str = "Settings";
	letters.renderLetters(temp_str,0,height-750,settings_size);
	temp_str = "Quit";
	letters.renderLetters(temp_str,0,height-900,quit_size);

	return 0;
}