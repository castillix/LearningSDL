#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine() {}

SnakeEngine::~SnakeEngine() {
	if(mRenderer)	SDL_DestroyRenderer(mRenderer);
        if(mWindow)	SDL_DestroyWindow(mWindow);
}

bool SnakeEngine::init() {
	if(!SDL_CreateWindowAndRenderer("Snake Game", kScreenWidth, kScreenHeight, 0, &mWindow, &mRenderer)) {
		SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool SnakeEngine::handleInputs(SDL_Event &e, bool &running) {
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_EVENT_QUIT) {
			running = false;
		}
	}
}

void SnakeEngine::run() {
	bool running{true};
	SDL_Event e;
	SDL_zero(e);

	while(running) {
		handleInputs(e, running);
	}
}