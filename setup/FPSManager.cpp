#include "FPSManager.h"
#include <thread>

FPSManager::FPSManager(double targetFPS) : deltaTime(0.0), frames(0), fps(0.0), frameDelay(1.0 / targetFPS) {
    lastTime = std::chrono::high_resolution_clock::now();
}

void FPSManager::startFrame() {
    currentTime = std::chrono::high_resolution_clock::now();
}

void FPSManager::endFrame() {
    frames++;

    deltaTime += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - currentTime).count();

    if (deltaTime >= 1.0) {
        fps = frames;
        std::cout << "FPS: " << fps << std::endl;
        frames = 0;
        deltaTime = 0;
    }

    double frameTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - currentTime).count();

    if (frameTime < frameDelay) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((frameDelay - frameTime) * 1000)));
    }
}

double FPSManager::getFPS() const {
    return fps;
}
