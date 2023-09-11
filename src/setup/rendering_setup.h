#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <iostream>
#include <cstdlib>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper();
	X11_wrapper(int w, int h);
	~X11_wrapper();

	void set_title();
	void check_resize(XEvent *e);
	void reshape_window(int width, int height);
	void setup_screen_res(const int w, const int h);
	void swapBuffers();
	bool getXPending();
	XEvent getXNextEvent();
	void set_mouse_position(int x, int y);
	void show_mouse_cursor(const int onoff);
};

void init_opengl(void);


#endif