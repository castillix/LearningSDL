#pragma once
#include <SDL3/SDL.h>
#include <cstring>

namespace Core {
    class InputManager {
    public:
        InputManager();
        
        // Call before event pumping
        void updateBegin();
        
        // Pass events to track explicit state if necessary (mouse, etc.)
        void handleEvent(const SDL_Event& e);
        
        // Query keys
        bool isKeyHeld(SDL_Scancode key) const;
        bool isKeyJustPressed(SDL_Scancode key) const;
        bool isKeyJustReleased(SDL_Scancode key) const;

        // Query mouse
        bool isMouseHeld(int button) const;
        bool isMouseJustPressed(int button) const;
        bool isMouseJustReleased(int button) const;
        void getMousePosition(float& x, float& y) const;
        void getMouseDelta(float& dx, float& dy) const;

    private:
        bool m_currentKeys[SDL_SCANCODE_COUNT];
        bool m_previousKeys[SDL_SCANCODE_COUNT];

        Uint32 m_currentMouseState;
        Uint32 m_previousMouseState;
        float m_mouseX, m_mouseY;
        float m_mouseDeltaX, m_mouseDeltaY;
    };
}
