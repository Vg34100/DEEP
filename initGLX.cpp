//initGLX.cpp
#include "initGLX.h"
#include "fonts.h"
#include <iostream>
#include <vector>
#include "prodriguezqu.h"
#include "apacheco2.h"

Display *dpy;
Window window;
GLXContext ctx;
bool isFullscreen = false;
// bool keysPressed[256] = {false};
float width = 1280;
float height = 720;
float aspect = (float)width / (float)height;
const float gameWidth = 1280.0f;  // Width of the game world in game units
const float gameHeight = gameWidth / aspect;
float mousex;
float mousey;
Atom wmDeleteMessage;  // Define as a global variable
Colormap cmap;
bool done = false;
struct ScreenSize {
	int width;
	int height;
};

const std::vector<ScreenSize> screenSizes {
	{3840, 2160},
	{3440, 1440},
	{2560, 1080},
	{2560, 1440},
	{1920, 1080},
	{1920, 1200},
	{1680, 1050},
	{1600, 900},
	{1366, 768},
	{1280, 720},
	{1024, 768},
	// Add other sizes as needed
};

int currentIndex = 4; // Start with the default size
void changeScreenSize(bool next) {
	if (next) {
		currentIndex = (currentIndex + 1) % screenSizes.size();
	} else {
		currentIndex = (currentIndex - 1 + screenSizes.size()) % screenSizes.size();
	}
	
	const auto& newSize = screenSizes[currentIndex];
	XResizeWindow(dpy, window, newSize.width, newSize.height);
	width = newSize.width;
	height = newSize.height;
}



int prevX, prevY, prevWidth, prevHeight; // Store the previous position and size of the window

void toggleFullscreen() {
	if (!isFullscreen) {
		// Store the previous size and position of the window
		XWindowAttributes windowAttributes;
		XGetWindowAttributes(dpy, window, &windowAttributes);
		prevX = windowAttributes.x;
		prevY = windowAttributes.y;
		prevWidth = windowAttributes.width;
		prevHeight = windowAttributes.height;

		// Move and resize the window to cover the entire screen
		XMoveResizeWindow(dpy, window, 0, 0, XDisplayWidth(dpy, 0), XDisplayHeight(dpy, 0));
	} else {
		// Restore the window to its previous size and position
		XMoveResizeWindow(dpy, window, prevX, prevY, prevWidth, prevHeight);
	}
	isFullscreen = !isFullscreen;
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(-gameWidth, gameWidth, -gameHeight, gameHeight, -1 ,1);  // Centered at (0, 0)

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initializeGLX() {
	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	XSetWindowAttributes swa;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XWindowAttributes getWinAttr;
	XGetWindowAttributes(dpy, root, &getWinAttr);

	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
		StructureNotifyMask | SubstructureNotifyMask;

	unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
	if (isFullscreen) {
		winops |= CWOverrideRedirect;
		swa.override_redirect = True;
	}

	window = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, winops, &swa);

	XMapWindow(dpy, window);
	XStoreName(dpy, window, "2D Top Down Game");

	ctx = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, window, ctx);

	XFree(vi);
	wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, window, &wmDeleteMessage, 1);

	//Disallow resizing
	XSizeHints* sizeHints = XAllocSizeHints();
	sizeHints->flags = PMinSize | PMaxSize;
	sizeHints->min_width = sizeHints->max_width = width;
	sizeHints->min_height = sizeHints->max_height = height;
	XSetWMNormalHints(dpy, window, sizeHints);
	XFree(sizeHints);

	initialize_fonts();
}

void cleanupGLX() {
	cleanup_fonts();
	glFlush();
	glFinish();
	if (dpy) {
		if (ctx) {
			glXMakeCurrent(dpy, None, NULL);
			glXDestroyContext(dpy, ctx);
			ctx = NULL; // Set to NULL to avoid accidental reuse
		}
		
		if (window) {
			XDestroyWindow(dpy, window);
			window = 0; // Set to 0 to avoid accidental reuse
		}
		
		if (cmap) {
			XFreeColormap(dpy, cmap);
			cmap = 0; // Set to 0 to avoid accidental reuse
		}
		
		XCloseDisplay(dpy);
		dpy = NULL; // Set to NULL to avoid accidental reuse
	}
}

bool keysPressed[65536] = {false}; // Increased size to accommodate KeySym values

void XPendingEvent(XEvent event) {
	mouse_since_counter(false, false);
	if (dpy == NULL) return; // Check if dpy is not NULL before proceeding
	while (XPending(dpy) > 0) {
		XNextEvent(dpy, &event);
		int key = (XLookupKeysym(&event.xkey, 0) & 0x0000ffff);
		switch (event.type) {
			case KeyPress: {
				keysPressed[key] = true;
				keyCheck = false;
                break;
			}
			case KeyRelease: {
				keysPressed[key] = false;
				keyCheck = true;
                break;
			}
			case MotionNotify: {
				mouse_since_counter(true, false);
				mousex = event.xmotion.x;
				mousey = event.xmotion.y;              
				break;
			}
			case ConfigureNotify: {
				break;
			}
			default:
				break;
		}
	}
	// In your event handling
	if ((event.type == ClientMessage && (Atom)event.xclient.data.l[0] == wmDeleteMessage)) {
	//if (keysPressed[XK_Escape] || (event.type == ClientMessage && (Atom)event.xclient.data.l[0] == wmDeleteMessage)) {
		done = true;
	}
}

void XReset() {
	glXSwapBuffers(dpy, window);
	glClear(GL_COLOR_BUFFER_BIT);
	fflush(stdout);
}
