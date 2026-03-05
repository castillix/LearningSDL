#pragma once
#include <SDL3/SDL.h>
#include "Engine/Core/Window.hpp"

namespace Graphics {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        bool init(Core::Window* window);
        void close();

        void beginFrame();
        void endFrame();

        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void drawRect(const SDL_FRect& rect);
        void fillRect(const SDL_FRect& rect);

        SDL_Renderer* getSDLRenderer() const { return m_renderer; }

    private:
        SDL_Renderer* m_renderer;
    };
}
