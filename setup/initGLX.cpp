//initGLX.cpp
#include "initGLX.h"
#include <iostream>


Display *dpy;
Window window;
GLXContext ctx;
bool isFullscreen = false;
bool keysPressed[256] = {false};
float width = 1280;
float height = 720;
float aspect = (float)width / (float)height;
const float gameWidth = 1280.0f;  // Width of the game world in game units
const float gameHeight = gameWidth / aspect;
float mousex;
float mousey;

void toggleFullscreen() {
    if (!isFullscreen) {
        XMoveResizeWindow(dpy, window, 0, 0, XDisplayWidth(dpy, 0), XDisplayHeight(dpy, 0));
    } else {
        XMoveResizeWindow(dpy, window, 0, 0, 600, 600);
    }
    isFullscreen = !isFullscreen;
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-gameWidth, gameWidth, -gameHeight, gameHeight);  // Centered at (0, 0)
}


void handleResize(XEvent *event) {
    int width = event->xconfigure.width;
    int height = event->xconfigure.height;
    aspect = (float)width / (float)height;
    float gameHeight = gameWidth / aspect;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gameWidth, gameWidth, -gameHeight, gameHeight);  // Centered at (0, 0)
}


void initializeGLX() {
    dpy = XOpenDisplay(NULL);

    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);

    Colormap cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    window = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap, &swa);
    
    XMapWindow(dpy, window);
    XStoreName(dpy, window, "2D Top Down Game");


    ctx = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, window, ctx);
}

void cleanupGLX() {
    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, ctx);
    XDestroyWindow(dpy, window);
    XCloseDisplay(dpy);
}

void XPendingEvent(XEvent event) {
    while (XPending(dpy) > 0) {
        XNextEvent(dpy, &event);
        int key = (XLookupKeysym(&event.xkey, 0) & 0x0000ffff);
        if (event.type == KeyRelease) {
            //gl.keys[key] = 0;
            if (key == XK_Shift_L || key == XK_Shift_R)
                std::cout << "CHECK CHECK" << std::endl;
        }


        switch (event.type) {
            case KeyPress: {
                char buf[2];
                KeySym keysym;
                XLookupString(&event.xkey, buf, sizeof(buf), &keysym, nullptr);
                keysPressed[char(buf[0])] = true;
                break;
            }
            case KeyRelease: {
                char buf[2];
                KeySym keysym;
                XLookupString(&event.xkey, buf, sizeof(buf), &keysym, nullptr);
                keysPressed[char(buf[0])] = false;
                break;
            }
            case MotionNotify: {
                mousex = event.xmotion.x;
                mousey = event.xmotion.y;              
                break;
            }
            case ConfigureNotify: {
                handleResize(&event);
                break;
            }
            default:
                break;
        }
    }
    if (keysPressed[27]) {  // ESC key to quit
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, ctx);
        XDestroyWindow(dpy, window);
        XCloseDisplay(dpy);
        exit(0);
    }
    if (keysPressed['f']) {  // 'f' key to toggle fullscreen
        toggleFullscreen();
    }
}

// void drawText(const char *text, int length, int x, int y) {
//     // Save current projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glPushMatrix();
//     glLoadIdentity();
//     glOrtho(-width, width, -height, height, -1, 1);

//     // Draw the text
//     glMatrixMode(GL_MODELVIEW);
//     glPushMatrix();
//     glLoadIdentity();
//     glRasterPos2i(x, y);
//     for(int i = 0; i < length; i++) {
//         glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
//     }
//     glPopMatrix();

//     // Restore the original projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glPopMatrix();

//     glMatrixMode(GL_MODELVIEW);  // Set the active matrix to modelview for further rendering
// }

