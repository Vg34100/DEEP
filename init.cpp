//init.cpp
#include <iostream>
#include <cmath>
#include <algorithm>

#include "src/setup/rendering_setup.h" // Defines the Wrapper
#include "src/setup/fonts.h" 
#include "src/setup/global.h" // Defines Global Variables

#include "src/player/player.h"
#include <chrono>
#include <unistd.h>


#include "src/common/bullet_manager.h"

#include "src/enemy/enemy.h"



using namespace std;

Global gl; // Defines Global Variables

enum class GameState {
    INIT,
    PLAYING,
    PAUSED,
    GAME_OVER
};

void startMenu();
bool isWithinPlayButton(int mouseX, int mouseY);
void initGame();
void check_keys(XEvent *e);
bool checkDistanceBasedCollision(float bulletX, float bulletY, float bulletRadius,
                                 float enemyX, float enemyY, float enemyHalfSize);

int main()
{

    GameState currentState = GameState::INIT;

    X11_wrapper x11(gl.xres, gl.yres);
    init_opengl();      // Initialize OpenGL settings

    const double TIME_STEP = 1.0 / 60.0; // 60 updates per second
    double accumulatedTime = 0.0;
    auto currentTime = std::chrono::high_resolution_clock::now();

    int done = 0;
    XEvent e;
    while(!done) {
        while (x11.getXPending()) {
            e = x11.getXNextEvent();
            x11.check_resize(&e);  // Check if window was resized
            //done = check_keys(&e); // Check if any exit key was pressed
            check_keys(&e);
    


        }
        if (currentState == GameState::INIT) {
            startMenu();
                // Check for mouse click
            if (e.type == ButtonPress) {
                // Check if the mouse coordinates are within the "Play" button's bounds
                // If so, change the game state to PLAYING
                if (isWithinPlayButton(e.xbutton.x, e.xbutton.y)) {
                    sleep(1);
                    currentState = GameState::PLAYING;
                }
            }
        }

        if (currentState == GameState::PLAYING) {
            glClear(GL_COLOR_BUFFER_BIT);

            static bool executed = false;

            static BulletManager bulletManager;
            static Player player;
            static Pistol pistol(bulletManager);
            static std::vector<Enemy> enemies;
            if (!executed)
            {
                player.equipWeapon(&pistol);
                enemies.push_back(BasicEnemy());
                executed = true;
            }

    



            // Check for mouse movement
            if (e.type == MotionNotify) {
                player.updateDirection(e.xmotion.x, gl.yres - e.xmotion.y);
            }
            
        auto newTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration<double>(newTime - currentTime).count();
        currentTime = newTime;
        accumulatedTime += frameTime;


        while (accumulatedTime >= TIME_STEP) {
            // Handle input and update game logic
            player.handleInput();
            player.update();
            // player.move();
            // player.applyPhysics();

            bulletManager.updateBullets();

            for (Enemy& enemy : enemies) {
                for (Bullet& bullet : bulletManager.getBullets()) {
                    if (checkDistanceBasedCollision(bullet.getX(), bullet.getY(), bullet.getRadius(),
                                                    enemy.getX(), enemy.getY(), enemy.getSize() / 2.0f)) {
                        enemy.onCollision();
                        printf("Collided");
                        bullet.setCollided(true);
                    }
                }
            }
            bulletManager.cleanupBullets();
            // Remove dead enemies
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy &enemy) {
                    return enemy.IsDead();
                }), enemies.end());


            accumulatedTime -= TIME_STEP;
        }

        //player.handleInput();

        for (Enemy& enemy : enemies) {
            enemy.update(player.getX(), player.getY());
            enemy.render();
        }
        bulletManager.renderBullets();
        player.render();
        }


        //render();                  // Render the window contents
        x11.swapBuffers();        // Swap front and back buffers
    }
    cleanup_fonts();              // Clean up fonts
    //logClose();                   // Close the log file
    return 0;
}

void check_keys(XEvent *e) {
    if (e->type != KeyPress && e->type != KeyRelease) return;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        gl.keys[key] = 1;
    } else {
        gl.keys[key] = 0;
    }
    static bool restart = false;
    if(gl.keys[XK_Escape]  && !restart)
    {
        restart = true;
        main();
        exit(0);
    }
}


// Utility function to check circle-square collision based on distance
bool checkDistanceBasedCollision(float bulletX, float bulletY, float bulletRadius,
                                 float enemyX, float enemyY, float enemyHalfSize) {
    // Calculate the distance between the centers
    float distanceX = std::abs(bulletX - enemyX);
    float distanceY = std::abs(bulletY - enemyY);

    // If the bullet is too far on either axis, no collision
    if (distanceX > (enemyHalfSize + bulletRadius)) return false;
    if (distanceY > (enemyHalfSize + bulletRadius)) return false;

    // If the bullet's center is inside the square, it's a collision
    if (distanceX <= enemyHalfSize) return true;
    if (distanceY <= enemyHalfSize) return true;

    // Check corner collision by examining the distance from the bullet's center to the square's corner
    float cornerDistance = std::pow((distanceX - enemyHalfSize), 2) + 
                           std::pow((distanceY - enemyHalfSize), 2);

    return (cornerDistance <= std::pow(bulletRadius, 2));
}

void startMenu()
{
    int buttonX = gl.xres / 2 - 50;  // centered on screen
    int buttonY = gl.yres / 2 - 25;  
    int buttonWidth = 100;
    int buttonHeight = 50;

    glColor3f(0.7f, 0.7f, 0.7f); // Color of the button (light gray for example)
    glBegin(GL_QUADS);
        glVertex2f(buttonX, buttonY);
        glVertex2f(buttonX + buttonWidth, buttonY);
        glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
        glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

        int textX = gl.xres / 2 - 20;  // Adjust as needed for the text to be centered on the button
    int textY = gl.yres / 2;

    Rect r;
    r.bot = textY;
    r.left = textX;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffffff, "Play");
}

bool isWithinPlayButton(int mouseX, int mouseY) {
    // Define the position and size of the "Play" button
    int buttonX = gl.xres / 2 - 50;  // assuming the button is 100 units wide and centered on screen
    int buttonY = gl.yres / 2 - 25;  // assuming the button is 50 units tall and centered on screen
    int buttonWidth = 100;
    int buttonHeight = 50;

    // Check if the mouse coordinates are within the button's boundaries
    if (mouseX >= buttonX &&
        mouseX <= buttonX + buttonWidth &&
        mouseY >= buttonY &&
        mouseY <= buttonY + buttonHeight) {
        return true;
    }
    return false;
}

void initGame() 
{

}
