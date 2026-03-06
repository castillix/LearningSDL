#include "../Engine.hpp"

Engine::Engine() {
        // do stuff
}

Engine::~Engine() {
        // undo stuff
}

bool Engine::init() {
        // init stuff
        return true;
}

void Engine::run() {
        bool running{true};
        SDL_Event e;
        SDL_zero(e);

        while(running) {
                
        }
}