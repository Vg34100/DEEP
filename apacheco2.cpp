#include "apacheco2.h"
#include "initGLX.h"
#include <ctime>
#include <iostream>
#include "fonts.h"
#include "prodriguezqu.h"
/**
 * Author: Aaron Pacheco
 * Created: 9.29.23 
*/
using namespace std;

bool keyCheck = false;


/*int total_running_time(const bool get)
{
    static int firsttime = 1;
    static int start_time;
    if (firsttime) {
        start_time = time(NULL);
        firsttime = 0;
    }
    if (get) {
        return time(NULL) - start_time;
    }
    return 0;

} */
int time_since_key_press(const bool get) {
    
    static int firsttime = 1;
    static int start_time;
    static int delta = 0;

    renderRect("Time since key press", delta);
    //initializes first time
    if (firsttime || !get) { 
        start_time = time(NULL);
        delta = 0;
        firsttime = 0;
    }
    //resets time if key pressed
    if (get) {
    //    cout << "yey" << endl;
        delta = time(NULL) - start_time;
        return delta;
    }
    
   
    return 0;

}
