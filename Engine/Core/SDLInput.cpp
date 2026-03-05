#include "Engine/Core/SDLInput.hpp"
#include "Engine/Interfaces/IEngine.hpp"
#include "Engine/Interfaces/IWindow.hpp"
#include <imgui_impl_sdl3.h>

namespace Engine {
namespace Core {

    void SDLInput::updateBegin() {
        m_previousKeys = m_currentKeys;
        m_previousMouse = m_currentMouse;
        m_deltaX = 0.0f;
        m_deltaY = 0.0f;
    }

    bool SDLInput::isKeyHeld(int keycode) const {
        auto it = m_currentKeys.find(keycode);
        if (it != m_currentKeys.end()) return it->second;
        return false;
    }

    bool SDLInput::isKeyJustPressed(int keycode) const {
        bool current = isKeyHeld(keycode);
        bool previous = false;
        auto it = m_previousKeys.find(keycode);
        if (it != m_previousKeys.end()) previous = it->second;
        return current && !previous;
    }

    bool SDLInput::isMouseHeld(int button) const {
        auto it = m_currentMouse.find(button);
        if (it != m_currentMouse.end()) return it->second;
        return false;
    }

    bool SDLInput::isMouseJustPressed(int button) const {
        bool current = isMouseHeld(button);
        bool previous = false;
        auto it = m_previousMouse.find(button);
        if (it != m_previousMouse.end()) previous = it->second;
        return current && !previous;
    }

    void SDLInput::getMouseDelta(float& dx, float& dy) const {
        dx = m_deltaX;
        dy = m_deltaY;
    }

    void SDLInput::pollEvents(Interfaces::IEngine* engine) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            handleEvent(e, engine);
        }
    }

    void SDLInput::handleEvent(const SDL_Event& e, Interfaces::IEngine* engine) {
        if (e.type == SDL_EVENT_QUIT) {
            engine->quit();
        } else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
            engine->quit();
        } else if (e.type == SDL_EVENT_KEY_DOWN) {
            m_currentKeys[e.key.scancode] = true;
        } else if (e.type == SDL_EVENT_KEY_UP) {
            m_currentKeys[e.key.scancode] = false;
        } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            m_currentMouse[e.button.button] = true;
        } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            m_currentMouse[e.button.button] = false;
        } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
            m_mouseX = e.motion.x;
            m_mouseY = e.motion.y;
            m_deltaX += e.motion.xrel;
            m_deltaY += e.motion.yrel;
        }
    }

}
}
