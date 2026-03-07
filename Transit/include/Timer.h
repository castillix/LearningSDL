#pragma once

#include <cstdint>

class Timer {
public:
        Timer();

        // update deltaTime for new frame
        void update();

        // milliseconds since program start
        uint64_t time() const;
        uint64_t deltaTime() const;

private:
        uint64_t startTime;
        uint64_t lastFrameTime;
        uint64_t deltaTime;
};