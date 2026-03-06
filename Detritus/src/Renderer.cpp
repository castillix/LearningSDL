#include "../Renderer.hpp"

Renderer::~Renderer() {
	if(mRenderer)	SDL_DestroyRenderer(mRenderer);
        if(mWindow)	SDL_DestroyWindow(mWindow);
}

bool Renderer::init() {
	if(!SDL_CreateWindowAndRenderer("Detritus", kScreenWidth, kScreenHeight, 0, &mWindow, &mRenderer)) {
		SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void Renderer::drawFrame(Engine& engine) {
        // insert code here

	SDL_RenderPresent(mRenderer);
}