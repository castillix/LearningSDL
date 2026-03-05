#pragma once
#include "Engine/Core/Time.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Core/InputManager.hpp"

namespace Entities {
    class Entity {
    public:
        virtual ~Entity() = default;

        virtual void update(float deltaTime, const Core::InputManager& input) = 0;
        virtual void render(Graphics::Renderer* renderer) = 0;
        
        void destroy() { m_isDestroyed = true; }
        bool isDestroyed() const { return m_isDestroyed; }

    protected:
        bool m_isDestroyed = false;
    };
}
