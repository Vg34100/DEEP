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

            physicsCountdown += timeSpan;
            while (physicsCountdown >= physicsRate) {
                player.handleInput();
                cm.handlePlayerCollisions(player);
                cm.handleEnemyCollisions(player);
                physicsCountdown -= physicsRate;
            }
            player.cameraSetup();
            world.render();
            world.renderEnemies();
            player.render();
            levelenemyText();

            glPopMatrix();
        }
    }
    cleanupGLX();
    //draw the statistics box
    if(gl.statistics) {
        gl.color3ub(100, 100, 100);
        glPushMaxtrix();
        glTranslatef(20.0, 20.0, 0.0);
        int w = 230;
        int h = 130;
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(0, h);
            glVertex2f(w, h);
            glVertex2f(w, 0);
        glEnd(0);
        r.bot = 124;
        r.left = 28;
        r.center = 0;
        ggprint13(&r, 20, 0x0055ff55, "Game Statistics");
        ggprint13(&r, 20, 0x00ffff00, "Runtime(sec): %i");
        ggprint13();
        ggprint13();
        ggprint13();
    }

    return 0;
  
}