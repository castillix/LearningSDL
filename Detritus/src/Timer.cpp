#include "../Timer.hpp"

Timer::Timer():
        startTime{SDL_GetTicks()},
        lastFrameTime{startTime},
        deltaTime{0} {}

void Timer::update() {
        Uint64 time = SDL_GetTicks();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
}

Uint64 Timer::time() const {
        return SDL_GetTicks() - startTime;
}

Uint64 Timer::deltaTime() const {
        return deltaTime;
}