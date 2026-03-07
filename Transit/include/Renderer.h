#pragma once

#include <SDL3/SDL.h>
#include <memory>

#include "Globals.hpp"
#include "Engine.hpp"


struct WindowDestroyer {
	void operator()(SDL_Window* window) const {
		if(window) SDL_DestroyWindow(window);
	}
};

struct RendererDestroyer {
	void operator()(SDL_Renderer* renderer) const {
		if(renderer) SDL_DestroyRenderer(renderer);
	}
};

class Renderer {
public:
        Renderer();
        ~Renderer() = default;

        bool init();
        void drawFrame(Engine& engine);

	SDL_Renderer* getSDLRenderer() const;

private:
	std::unique_ptr<SDL_Window, WindowDestroyer> mWindow;
	std::unique_ptr<SDL_Renderer, RendererDestroyer> mRenderer;

};