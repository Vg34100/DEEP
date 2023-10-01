//modified by: Pablo Rodriguez, Arron Pacheco, Vincent To, Justin Lo
//program: asteroids.cpp -> main.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
#include "setup/initGLX.h"
#include "player/player.h"
#include <chrono>
#include <thread>
#include <string>
#include <unistd.h>
#include "gui/titleScreen.h"
#include "world/world.h"
#include "enemies/enemy.h"
#include "common/CollisionManager.h"
#include "apacheco2.h"
#include "jlo.h"
#include "prodriguezqu.h"
#include "vto.h"

const double physicsRate = 1.0 / 60.0;
double physicsCountdown=0.0;
double timeSpan=0.0;

enum class GameState {
    INIT,
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
    printf("Objective: Kill Enemies (White) with Attack -> Proceed to Next Level through Hallway (LightGray)");
    fflush(stdout);

    auto lastUpdateTime = std::chrono::high_resolution_clock::now();



    while (!done) {
        XReset();
        XEvent event;
        XPendingEvent(event);       

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
        //auto deltaTime = std::chrono::duration<float>(elapsedTime).count(); // for future use if acceleartion based character is used
        lastUpdateTime = currentTime;
        physicsCountdown += elapsedTime;


        if (currentState == GameState::INIT) {
            titleScreen(width, height);
            if (currentState == GameState::INIT && keysPressed[XK_Return]) { //Enter key is pressed
                sleep(1);
                currentState = GameState::PLAYING; //Start the Game
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

            glPopMatrix();
        }
    }
    cleanupGLX();
    return 0;
}
