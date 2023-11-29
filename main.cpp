//modified by: Pablo Rodriguez, Arron Pacheco, Vincent To, Justin Lo
//program: asteroids.cpp -> main.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
#include "initGLX.h"
#include "player.h"
#include <chrono>
#include <cstring>
#include <thread>
#include <string>
#include <unistd.h>
#include "world.h"
#include "enemy.h"
#include "util_CollisionManager.h"
#include "apacheco2.h"
#include "jlo.h"
#include "prodriguezqu.h"
#include "vto.h"
#include "image.h"
#include "fonts.h"
#include "gui.h"
#include "util_keys.h"
#include "keybinds.h"

const double physicsRate = 1.0 / 256.0;
const double oobillion = 1.0 / 1e9;
double physicsCountdown=0.0;
double timeSpan=0.0;
struct timespec timeStart, timeCurrent;
struct timespec timePause;

double timeDiff(struct timespec *start, struct timespec *end)
{
	//return the difference in two times.
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source)
{
	//copy one time structure to another.
	memcpy(dest, source, sizeof(struct timespec));
}


 int total_running_time(const bool get)
{
	// printf("Running");
	static int firsttime = 1;
	static int start_time;
	if (firsttime || !get) {
		start_time = time(NULL);
		firsttime=0;
	}
	if (get) {
		// printf("%li", time(NULL) - start_time);
		return time(NULL) - start_time;
	}
	return 0;
} 



void devMode()
{
	static bool statsScreen;
	static int DMkeyHoldCounter = 0; //DM - DevMode
	const int DMkeyHoldThreshold = 30; 

	if (handleKeyAction(keysPressed[dev_key] && (keysPressed[l_control_key] || keysPressed[r_control_key]), DMkeyHoldCounter, DMkeyHoldThreshold))
    	statsScreen = !statsScreen;

	if(statsScreen) {
		levelenemyText(total_running_time(true));
		renderFunctionCalls(true);
		time_since_key_press(keyCheck);
	}
}


enum class GameState {
	INIT,
	OPTIONS,
	PLAYING,
	SHOP,
	PAUSED,
	GAME_OVER
};

int main() 
{
	GameState currentState = GameState::INIT;
	initializeGLX();
	initGL();
	playing_check = false;
	srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

	printf("Press Enter to Play\n");
	printf("Change Screen Size using left and right arrows\n");
	printf("Move - WASD | Attack - R | Aim - Mouse\n");
	printf("Objective: Kill Enemies (White) with Attack -> Proceed to Next Level through Hallway (LightGray)\n");
	printf("Player Stats wit CTRL+Z\n");
	printf("Display Stats and other Developer Options with CTRL+S\n");
	fflush(stdout);

	#ifdef DEBUG
	printf("DEBUG Active");
	#endif // DEBUG
	// auto lastUpdateTime = std::chrono::high_resolution_clock::now();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	static World world;
	static CollisionManager cm(world);
	static Player player(cm, 100.0f); 
	static GUI gui(world, player);
	static Shop shop(world, player);
	playing_check = false;
	while (!done) {
		XReset();
		XEvent event;
		XPendingEvent(event);       

		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		static int init_inputDelayCounter = 80;

		static bool paused_check = false;
		if (currentState == GameState::INIT) {
			paused_check = false;
			int ts_result = titleScreen();
			if (init_inputDelayCounter > 0) {
				init_inputDelayCounter--;
				continue;
			}
			switch(ts_result) {
				case 1:
					init_inputDelayCounter = 80;
					currentState = GameState::PLAYING;
					break;
				case 2:
					init_inputDelayCounter = 80;
					currentState = GameState::OPTIONS;
					break;
				case -1:
					done = true;
					break;
				default:
					break;
			}
		}

		if (currentState == GameState::OPTIONS) {
			switch(optionScreen()) {
				case -1: {
					if (paused_check)
						currentState = GameState::PAUSED;
					else {
						currentState = GameState::INIT;
					}

					break;
				}

			}
		}

		if (currentState == GameState::SHOP) {
			static int inputDelayCounter = 50;
			if (inputDelayCounter > 0) {
				inputDelayCounter--;
			}
			if (keysPressed[XK_Escape] && inputDelayCounter <= 0) {
				currentState = GameState::PLAYING;
				inputDelayCounter = 50;
			}
			shop.render(timeSpan);







		}

		if (currentState == GameState::PLAYING) {
			playing_check = true;
			static int inputDelayCounter = 50;
			if (inputDelayCounter > 0) {
				inputDelayCounter--;
			}
			if (keysPressed[XK_y]) {
				printf("Attempting to Restart...\n");
				Player player2(cm, 100);
				World world2;
			}
			if (keysPressed[XK_Escape] && inputDelayCounter <= 0) {
				currentState = GameState::PAUSED;
				inputDelayCounter = 50;
			} else if (keysPressed[XK_l] && inputDelayCounter <= 0) {
				currentState = GameState::SHOP;
				inputDelayCounter = 50;
			}


			player.cameraSetup();
			while (physicsCountdown >= physicsRate) {
				player.handleInput();
				cm.handlePlayerCollisions(player);
				cm.handleEnemyCollisions(player);
				player.update(timeSpan);
				physicsCountdown -= physicsRate;
			}
				for (const auto& enemy : world.getEnemies()) {
					enemy->moveToPlayer(player.getPos());
				}
				world.render();
				world.renderEnemies();
				player.render();
				player.animate(timeSpan * 80);
				gui.render(timeSpan);

			glPopMatrix();
		}

		if (currentState == GameState::PAUSED) {
			paused_check = true;
			static int inputDelayCounter = 80;
			int p_result = paused();
			if (inputDelayCounter > 0) {
				inputDelayCounter--;
				continue;
			}
			if (keysPressed[XK_Escape]) {
				inputDelayCounter = 80;
				usleep(1000);
				paused_check = false;
				currentState = GameState::PLAYING;
			}

			switch(p_result) {
			case -1: {
				inputDelayCounter = 80;
				usleep(1000);
				paused_check = false;
				currentState = GameState::PLAYING;	
				break;
			}
			case 0: {
				break;
			}
			case 1: {
				inputDelayCounter = 80;
				currentState = GameState::OPTIONS;
				break;
			}
			case 2: {
				inputDelayCounter = 80;
				currentState = GameState::INIT;
				break;
			}
			default: {
				break;
			}
			}

		}

		devMode();
	}
	cleanupGLX();
	return 0;
}
