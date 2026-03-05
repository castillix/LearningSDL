#include "Engine/Graphics/SDLWindow.hpp"
#include <glad/glad.h>
#include <iostream>

namespace Engine {
namespace Graphics {

    SDLWindow::~SDLWindow() {
        close();
    }

    bool SDLWindow::init(const std::string& title, int width, int height) {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_INIT_VIDEO failed: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
        if (!m_window) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }

        m_context = SDL_GL_CreateContext(m_window);
        if (!m_context) {
            std::cerr << "GL Context creation failed: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_GL_MakeCurrent(m_window, m_context);

        // Load Glad
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Glad initialization failed." << std::endl;
            return false;
        }

        return true;
    }

    void SDLWindow::close() {
        if (m_context) {
            SDL_GL_DestroyContext(m_context);
            m_context = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void SDLWindow::swapBuffers() {
        SDL_GL_SwapWindow(m_window);
    }

    void SDLWindow::setRelativeMouseMode(bool enabled) {
        SDL_SetWindowRelativeMouseMode(m_window, enabled);
    }

    void* SDLWindow::getNativeWindow() {
        return m_window;
    }

}
}
