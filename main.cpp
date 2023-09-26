//main.cpp
#include "setup/initGLX.h"
#include "setup/FPSManager.h"
#include "player/player.h"
#include <chrono>
#include <thread>
#include <string>
#include <unistd.h>
#include "gui/titleScreen.h"
#include "world/world.h"
#include "enemies/enemy.h"
#include "common/CollisionManager.h"
#include "devFiles/apacheco2.h"
#include "devFiles/jlo.h"
#include "devFiles/prodriguezqu.h"
#include "devFiles/vto.h"


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

    World world;
    // This line ensures we're listening to keyboard inputs.
    XSelectInput(dpy, window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | StructureNotifyMask);

    FPSManager fpsManager(60.0);  // Target 60 FPS

    while (true) {
        XEvent event;
        XPendingEvent(event);       
        glClear(GL_COLOR_BUFFER_BIT);

        if (currentState == GameState::INIT) {
            titleScreen(width, height);
            printf("%f,%f\n", mousex, mousey);
            fflush(stdout);
            if (currentState == GameState::INIT && keysPressed[13]) { //Enter key is pressed
                sleep(1);
                currentState = GameState::PLAYING; //Start the Game
            }
        }

        if (currentState == GameState::PLAYING) {

            static CollisionManager cm(world);
            static Player player(cm, 100.0f); 
            fpsManager.startFrame();

            player.handleInput();
            player.cameraSetup();
            cm.handlePlayerCollisions(player);
            cm.handleEnemyCollisions(player);
            world.render();
            world.renderEnemies();
            player.render();
            //Restore original matrix
            glPopMatrix();

            // Continue with other game rendering if any...

            fpsManager.endFrame();
        }
        glXSwapBuffers(dpy, window);
    }
    cleanupGLX();
    return 0;
}
