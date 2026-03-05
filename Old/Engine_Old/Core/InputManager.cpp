#include "Engine/Core/InputManager.hpp"
#include <algorithm>

namespace Core {
    InputManager::InputManager() : m_currentMouseState(0), m_previousMouseState(0), m_mouseX(0), m_mouseY(0), m_mouseDeltaX(0), m_mouseDeltaY(0) {
        std::fill(m_currentKeys, m_currentKeys + SDL_SCANCODE_COUNT, false);
        std::fill(m_previousKeys, m_previousKeys + SDL_SCANCODE_COUNT, false);
    }

    void InputManager::updateBegin() {
        // Copy current states to previous
        std::copy(m_currentKeys, m_currentKeys + SDL_SCANCODE_COUNT, m_previousKeys);
        m_previousMouseState = m_currentMouseState;

        // Fetch current keyboard state
        int numkeys;
        const bool* state = SDL_GetKeyboardState(&numkeys);
        for (int i = 0; i < numkeys && i < SDL_SCANCODE_COUNT; ++i) {
            m_currentKeys[i] = state[i];
        }

        // Fetch current mouse state
        m_currentMouseState = SDL_GetMouseState(&m_mouseX, &m_mouseY);
        SDL_GetRelativeMouseState(&m_mouseDeltaX, &m_mouseDeltaY);
    }

    void InputManager::handleEvent(const SDL_Event& e) {
        // Here we could handle continuous wheel events or specific text input events 
        // that are not covered by GetKeyboardState / GetMouseState
    }

    bool InputManager::isKeyHeld(SDL_Scancode key) const {
        return m_currentKeys[key];
    }

    bool InputManager::isKeyJustPressed(SDL_Scancode key) const {
        return m_currentKeys[key] && !m_previousKeys[key];
    }

    bool InputManager::isKeyJustReleased(SDL_Scancode key) const {
        return !m_currentKeys[key] && m_previousKeys[key];
    }

    bool InputManager::isMouseHeld(int button) const {
        return (m_currentMouseState & SDL_BUTTON_MASK(button)) != 0;
    }

    bool InputManager::isMouseJustPressed(int button) const {
        return ((m_currentMouseState & SDL_BUTTON_MASK(button)) != 0) &&
               ((m_previousMouseState & SDL_BUTTON_MASK(button)) == 0);
    }

    bool InputManager::isMouseJustReleased(int button) const {
        return ((m_currentMouseState & SDL_BUTTON_MASK(button)) == 0) &&
               ((m_previousMouseState & SDL_BUTTON_MASK(button)) != 0);
    }

    void InputManager::getMousePosition(float& x, float& y) const {
        x = m_mouseX;
        y = m_mouseY;
    }

    void InputManager::getMouseDelta(float& dx, float& dy) const {
        dx = m_mouseDeltaX;
        dy = m_mouseDeltaY;
    }
}
