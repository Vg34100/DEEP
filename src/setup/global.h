// Defines Global Variables
#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstring>

class Global {
public:
    int xres, yres;
    char keys[65536];
    Global() {
        xres = 1280;
        yres = 720;
        memset(keys, 0, 65536);
    }
};

extern Global gl;

#endif