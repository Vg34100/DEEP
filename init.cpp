#include <iostream>
#include "src/setup/rendering_setup.h" // Defines the Wrapper
#include "src/setup/fonts.h" 
#include "src/setup/global.h" // Defines Global Variables

#include "src/player/player.h"
#include <chrono>


using namespace std;

Global gl; // Defines Global Variables

void check_keys(XEvent *e);

int main()
{
    X11_wrapper x11(gl.xres, gl.yres);
    init_opengl();      // Initialize OpenGL settings

    Player player;
    const double TIME_STEP = 1.0 / 60.0; // 60 updates per second
    double accumulatedTime = 0.0;
    auto currentTime = std::chrono::high_resolution_clock::now();

    int done = 0;
    while(!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);  // Check if window was resized
            //done = check_keys(&e); // Check if any exit key was pressed
            check_keys(&e);
    
            // Check for mouse movement
            if (e.type == MotionNotify) {
                player.updateDirection(e.xmotion.x, gl.yres - e.xmotion.y);
            }

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

            accumulatedTime -= TIME_STEP;
        }

        //player.handleInput();

        glClear(GL_COLOR_BUFFER_BIT);
        player.render();

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
}
