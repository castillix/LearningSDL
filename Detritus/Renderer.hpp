#pragma once

#include <SDL3/SDL.h>

#include "Globals.hpp"
#include "Engine.hpp"

class Renderer {
public:
        Renderer();
        ~Renderer();

        void drawFrame(Engine& engine);

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

        bool init();
};