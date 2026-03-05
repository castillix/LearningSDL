#pragma once
#include "Engine/Entities/Entity.hpp"
#include "Engine/Game/Terrain.hpp"
#include <functional>

namespace Entities {
    class Projectile : public Entity {
    public:
        using ExplodeCallback = std::function<void(float, float, int)>;
        Projectile(Game::Terrain* terrain, float startX, float startY, float vx, float vy, ExplodeCallback onExplode = nullptr);

        void update(float deltaTime, const Core::InputManager& input) override;
        void render(Graphics::Renderer* renderer) override;

    private:
        Game::Terrain* m_terrain;
        ExplodeCallback m_onExplode;
        float m_x, m_y;
        float m_vx, m_vy;

        const float GRAVITY = 300.0f; // Slower arc for better gameplay readability
        const int EXPLOSION_RADIUS = 30;
    };
}
