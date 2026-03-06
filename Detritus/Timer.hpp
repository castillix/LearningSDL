#pragma once

#include <SDL3/SDL.h>

class Timer {
public:
        Timer();

        // update deltaTime for new frame
        void update();

        // milliseconds since program start
        Uint64 time() const;
        Uint64 deltaTime() const;

private:
        Uint64 startTime;
        Uint64 lastFrameTime;
        Uint64 deltaTime;
};