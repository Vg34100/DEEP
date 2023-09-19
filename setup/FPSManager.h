#pragma once

#include <chrono>
#include <iostream>

class FPSManager {
private:
    std::chrono::high_resolution_clock::time_point currentTime, lastTime;
    double deltaTime;
    long frames;
    double fps;
    const double frameDelay;

public:
    FPSManager(double targetFPS);
    void startFrame();
    void endFrame();
    double getFPS() const;
};


