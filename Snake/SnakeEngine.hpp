#pragma once
#include <SDL3/SDL.h>
#include "Globals.hpp"

class SnakeEngine {
public:
	SnakeEngine();
	~SnakeEngine();

	SnakeEngine(const SnakeEngine&) = delete;
        SnakeEngine& operator=(const SnakeEngine&) = delete;

	bool init();
	void run();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	bool handleInputs(SDL_Event &e, bool &running);
};