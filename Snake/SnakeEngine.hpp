#pragma once

#include <SDL3/SDL.h>
#include <vector>

#include "Globals.hpp"
#include "Snake.hpp"
#include "Grid.hpp"

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

	Grid mGrid;
	Snake mSnake;

	bool handleInputs(SDL_Event& e);
	void drawScreen();
};