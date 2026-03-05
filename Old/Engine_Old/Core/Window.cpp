#include <glad/glad.h>
#include "Engine/Core/Window.hpp"

namespace Core {
    Window::Window(const std::string& title, int width, int height)
        : m_title(title), m_width(width), m_height(height), m_window(nullptr) {}

    Window::~Window() {
        close();
    }

    bool Window::init() {
        // Request OpenGL 3.3 Core Profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_window = SDL_CreateWindow(m_title.c_str(), m_width, m_height, SDL_WINDOW_OPENGL);
        if (!m_window) {
            SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            SDL_Log("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            SDL_Log("Failed to initialize GLAD\n");
            return false;
        }
        
        glEnable(GL_DEPTH_TEST);
        SDL_GL_SetSwapInterval(1); // Enable VSync

        return true;
    }

    void Window::close() {
        if (m_glContext) {
            SDL_GL_DestroyContext(m_glContext);
            m_glContext = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void Window::swapBuffers() {
        if (m_window) {
            SDL_GL_SwapWindow(m_window);
        }
    }
}
