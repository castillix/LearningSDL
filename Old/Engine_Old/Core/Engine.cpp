#include "Engine/Core/Engine.hpp"
#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Core {
    Engine::Engine(const std::string& title, int width, int height)
        : m_window(title, width, height), m_isRunning(false) {}

    Engine::~Engine() {
        if (m_game) {
            m_game->cleanup();
        }
        m_textures.cleanup();
        m_renderer.close();
        m_window.close();
        SDL_Quit();
    }

    bool Engine::init(std::unique_ptr<Game::IGame> game) {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
            return false;
        }

        if (!m_window.init()) return false;
        if (!m_renderer.init(&m_window)) return false;
        
        // Ensure Depth Testing is enabled for 3D
        glEnable(GL_DEPTH_TEST);
        
        if (!m_audio.init()) return false;
        
        m_textures.init(&m_renderer);

        m_game = std::move(game);
        if (m_game) {
            if (!m_game->init(&m_renderer, &m_textures, &m_audio)) {
                return false;
            }
        }

        m_isRunning = true;
        return true;
    }

    void Engine::run() {
        while (m_isRunning) {
            m_time.update();
            m_input.updateBegin();
            handleEvents();

            if (m_game) {
                m_game->update(m_time.getDeltaTime(), m_input);
                m_game->render(&m_renderer);
            }

            m_window.swapBuffers();
        }
    }

    void Engine::handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                m_isRunning = false;
            } else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
                m_isRunning = false;
            }
            m_input.handleEvent(e);
        }
    }
}
