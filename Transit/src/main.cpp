#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Engine.hpp"

void run() {
	Engine engine;
	if(engine.init()) {
		engine.run();
	}
}

int main(int argc, char* args[]) {
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL could not initialize! SDL error: %S\n", SDL_GetError());
		return 1;
	}

	run();

	SDL_Quit();

	return 0;
}