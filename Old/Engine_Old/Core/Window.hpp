#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace Core {
    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool init();
        void close();
        void swapBuffers();

        SDL_Window* getSDLWindow() const { return m_window; }
        SDL_GLContext getGLContext() const { return m_glContext; }
        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

    private:
        std::string m_title;
        int m_width;
        int m_height;
        SDL_Window* m_window;
        SDL_GLContext m_glContext = nullptr;
    };
}
