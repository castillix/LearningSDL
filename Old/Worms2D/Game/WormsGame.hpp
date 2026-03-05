#pragma once
#include "Engine/Game/IGame.hpp"
#include "Engine/Entities/EntityManager.hpp"
#include "Terrain.hpp"
#include "TurnManager.hpp"

namespace Entities {
    class Worm;
}

namespace Game {
    class WormsGame : public IGame {
    public:
        WormsGame();
        ~WormsGame() override = default;

        bool init(Graphics::Renderer* renderer, Graphics::TextureManager* textures, Audio::SoundManager* audio) override;
        void update(float deltaTime, const Core::InputManager& input) override;
        void render(Graphics::Renderer* renderer) override;
        void cleanup() override;

    private:
        Entities::EntityManager m_entityManager;
        Terrain m_terrain;
        TurnManager m_turnManager;
        
        Graphics::TextureManager* m_textures = nullptr;
        Audio::SoundManager* m_audio = nullptr;
    };
}
