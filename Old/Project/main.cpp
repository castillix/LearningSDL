//Using SDL, SDL_image, and STL string
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <ctime>
#include "Globals.hpp"
#include "DVDLogo.hpp"

SDL_Window* gWindow{nullptr};
SDL_Renderer* gRenderer{nullptr};
DVDLogo gLogo;

/* Function Implementations */
bool init() {
	bool success{true};

	// init SDL
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		success = false;
	} else {
		// Create window with renderer
		if(SDL_CreateWindowAndRenderer("SDL3 Tutorial: Textures and Extension Libraries", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer) == false) {
			SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
	}
	return success;
}

void close() {
	gLogo.destroy();

	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_Quit();
}

int main(int argc, char* args[]) {
	int exitCode{0};

	if(!init()) {
		SDL_Log("Unable to init program!\n");
		exitCode = 1;
	} else {
		if(!gLogo.init()) {
			SDL_Log("Unable to load media!\n");
			exitCode = 2;
		} else {
			bool running{true};

			SDL_Event e;
			SDL_zero(e);

			while(running) {
				while(SDL_PollEvent(&e)) {
					if(e.type == SDL_EVENT_QUIT) {
						running = false;
					}
				}

				gLogo.advance();

				SDL_Color color = gLogo.getRGB();
				
				SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
				SDL_RenderClear(gRenderer);

				gLogo.render();

				SDL_RenderPresent(gRenderer);
				SDL_Delay(10);
			}
		}
	}

	close();

	return exitCode;
}