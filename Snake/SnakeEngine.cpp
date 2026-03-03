#include "SnakeEngine.hpp"

SnakeEngine::SnakeEngine():
	mGrid{kGridWidth, kGridHeight},
	mSnake{kGridWidth / 2, kGridHeight / 2, mGrid} {}

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

void SnakeEngine::run() {
	bool running{true};
	SDL_Event e;
	SDL_zero(e);

	while(running) {
		running = handleInputs(e);
		running = running && mSnake.advance(mGrid) == 0;
		drawScreen();

		SDL_Delay(100);
	}
}

bool SnakeEngine::handleInputs(SDL_Event& e) {
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_EVENT_QUIT) {
			return false;
		} else if(e.type == SDL_EVENT_KEY_DOWN) {
			if(e.key.key == SDLK_UP) {
				mSnake.setDirection(2);
			} else if(e.key.key == SDLK_RIGHT) {
				mSnake.setDirection(1);
			} else if(e.key.key == SDLK_DOWN) {
				mSnake.setDirection(0);
			} else if(e.key.key == SDLK_LEFT) {
				mSnake.setDirection(3);
			}
		}
	}
	return true;
}

void SnakeEngine::drawScreen() {
	SDL_SetRenderDrawColor(mRenderer, 10, 10, 10, 255);
	SDL_RenderClear(mRenderer);

	float cellW = static_cast<float>(kScreenWidth) / kGridWidth;
	float cellH = static_cast<float>(kScreenHeight) / kGridHeight;
	
	for(int x = 0; x < kGridWidth; x++) {
		for(int y = 0; y < kGridHeight; y++) {
			int val = mGrid.get(x, y);
			// if(val != 0) {
				if(val == 1) {
					SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
				} else if(val == 2) {
					SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
				} else {
					SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
				}

				SDL_FRect rect;
				rect.w = cellW - 1;
				rect.h = cellH - 1;
				rect.x = x * cellW;
				rect.y = y * cellH;

				SDL_RenderFillRect(mRenderer, &rect);
			// }
		}
	}

	SDL_RenderPresent(mRenderer);
}