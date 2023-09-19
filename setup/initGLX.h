#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h> // For window size hints

extern Display *dpy;
extern Window window;
extern GLXContext ctx;

void initializeGLX();
void initGL();
void cleanupGLX();
void toggleFullscreen();
void handleResize(XEvent *event);
void XPendingEvent(XEvent event);

extern bool keysPressed[256];
extern float width;
extern float height;
extern float mousex;
extern float mousey;

void drawText(const char *text, int length, int x, int y, float scale = 1, int thickness = 1);
void drawWorldText(const char *text, int length, int x, int y, float scale = 1, int thickness = 1);
