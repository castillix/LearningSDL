#pragma once
#include "Engine/Interfaces/IInput.hpp"
#include <SDL3/SDL.h>
#include <unordered_map>

namespace Engine {
namespace Core {

    class SDLInput : public Interfaces::IInput {
    public:
        SDLInput() = default;
        ~SDLInput() override = default;

        void updateBegin() override;
        bool isKeyHeld(int keycode) const override;
        bool isKeyJustPressed(int keycode) const override;
        bool isMouseHeld(int button) const override;
        bool isMouseJustPressed(int button) const override;
        void getMouseDelta(float& dx, float& dy) const override;
        
        void pollEvents(class Interfaces::IEngine* engine) override;

    private:
        std::unordered_map<int, bool> m_currentKeys;
        std::unordered_map<int, bool> m_previousKeys;
        
        std::unordered_map<int, bool> m_currentMouse;
        std::unordered_map<int, bool> m_previousMouse;

        float m_mouseX = 0.0f;
        float m_mouseY = 0.0f;
        float m_deltaX = 0.0f;
        float m_deltaY = 0.0f;

        void handleEvent(const SDL_Event& e, class Interfaces::IEngine* engine);
    };

}
}
