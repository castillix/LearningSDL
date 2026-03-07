#include "Timer.hpp"

Timer::Timer():
        startTime{SDL_GetTicks()},
        lastFrameTime{startTime},
        deltaTime{0} {}

void Timer::update() {
        Uint64 time = SDL_GetTicks();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
}

uint64_t Timer::time() const {
        return SDL_GetTicks() - startTime;
}

uint64_t Timer::deltaTime() const {
        return deltaTime;
}