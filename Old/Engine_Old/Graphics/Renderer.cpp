#include "Engine/Graphics/Renderer.hpp"

namespace Graphics {
    Renderer::Renderer() : m_renderer(nullptr) {}

    Renderer::~Renderer() {
        close();
    }

    bool Renderer::init(Core::Window* window) {
        if (!window || !window->getSDLWindow()) {
            return false;
        }

        m_renderer = SDL_CreateRenderer(window->getSDLWindow(), nullptr);
        if (!m_renderer) {
            SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }
        return true;
    }

    void Renderer::close() {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
    }

    void Renderer::beginFrame() {
        SDL_RenderClear(m_renderer);
    }

    void Renderer::endFrame() {
        SDL_RenderPresent(m_renderer);
    }

    void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }

    void Renderer::drawRect(const SDL_FRect& rect) {
        SDL_RenderRect(m_renderer, &rect);
    }

    void Renderer::fillRect(const SDL_FRect& rect) {
        SDL_RenderFillRect(m_renderer, &rect);
    }
}
