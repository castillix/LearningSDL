#include "Renderer.h"

Renderer::Renderer() {
	// nothing here yet!
}

Renderer::~Renderer() {
	if(mRenderer)	SDL_DestroyRenderer(mRenderer);
        if(mWindow)	SDL_DestroyWindow(mWindow);
}

bool Renderer::init() {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	if(!SDL_CreateWindowAndRenderer("Detritus", kScreenWidth, kScreenHeight, 0, &window, &renderer)) {
		SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
		return false;
	}

	mWindow.reset(window);
	mRenderer.reset(renderer);

	return true;
}

void Renderer::drawFrame(Engine& engine) {
	SDL_SetRenderDrawColor(mRenderer.get(), 0x00, 0x00, 0x00, 255);
	SDL_RenderClear(mRenderer.get());

	// draw frame

	SDL_RenderPresent(mRenderer);
}

SDL_Renderer* Renderer::getSDLRenderer() const {
	return mRenderer.get();
}