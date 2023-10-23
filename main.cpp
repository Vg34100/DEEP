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

const double physicsRate = 1.0 / 60.0;
double physicsCountdown=0.0;
double timeSpan=0.0;

enum class GameState {
	INIT,
	OPTIONS,
	PLAYING,
	PAUSED,
	GAME_OVER
};

int main() {
	GameState currentState = GameState::INIT;
	initializeGLX();
	initGL();


	printf("Press Enter to Play\n");
	printf("Change Screen Size using left and right arrows\n");
	printf("Move - WASD | Attack - R | Aim - Mouse\n");
	printf("Objective: Kill Enemies (White) with Attack -> Proceed to Next Level through Hallway (LightGray)\n");
	fflush(stdout);

	auto lastUpdateTime = std::chrono::high_resolution_clock::now();



	while (!done) {
		XReset();
		XEvent event;
		XPendingEvent(event);       

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
		lastUpdateTime = currentTime;
		physicsCountdown += elapsedTime;


		if (currentState == GameState::INIT) {
			switch(titleScreen()) {
				case 1:
					currentState = GameState::PLAYING;
					break;
				case 2:
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
				case -1:
					currentState = GameState::INIT;
					break;
			}
		}


		if (currentState == GameState::PLAYING) {
			static World world;
			static CollisionManager cm(world);
			static Player player(cm, 100.0f); 

			player.cameraSetup();
			physicsCountdown += timeSpan;
			while (physicsCountdown >= physicsRate) {
				player.handleInput();
				cm.handlePlayerCollisions(player);
				cm.handleEnemyCollisions(player);
				physicsCountdown -= physicsRate;
			}
				world.render();
				world.renderEnemies();
				player.render();
				player.animate(elapsedTime * 1000);
				levelenemyText();

			glPopMatrix();
		}
	}
	cleanupGLX();
	return 0;
}
