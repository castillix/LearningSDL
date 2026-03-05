#pragma once
#include "Engine/Interfaces/IWindow.hpp"
#include <SDL3/SDL.h>

namespace Engine {
namespace Graphics {

    class SDLWindow : public Interfaces::IWindow {
    public:
        SDLWindow() = default;
        ~SDLWindow() override;

        bool init(const std::string& title, int width, int height) override;
        void close() override;
        void swapBuffers() override;
        void setRelativeMouseMode(bool enabled) override;
        void* getNativeWindow() override;

    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;
    };

}
}
