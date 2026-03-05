#pragma once
#include "Engine/Core/InputManager.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/TextureManager.hpp"
#include "Engine/Audio/SoundManager.hpp"

namespace Game {
    class IGame {
    public:
        virtual ~IGame() = default;

        virtual bool init(Graphics::Renderer* renderer, Graphics::TextureManager* textures, Audio::SoundManager* audio) = 0;
        virtual void update(float deltaTime, const Core::InputManager& input) = 0;
        virtual void render(Graphics::Renderer* renderer) = 0;
        virtual void cleanup() = 0;
    };
}
