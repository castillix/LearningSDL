#pragma once
#include "Engine/Entities/Entity.hpp"
#include "Engine/Game/Terrain.hpp"

namespace Entities {
    class Worm : public Entity {
    public:
        // Pass terrain for collision and initial spawn coordinates
        Worm(Game::Terrain* terrain, float startX, float startY);

        void update(float deltaTime, const Core::InputManager& input) override;
        void render(Graphics::Renderer* renderer) override;

        float getX() const { return m_x; }
        float getY() const { return m_y; }

        void takeDamage(int amount);
        int getHealth() const { return m_health; }
        bool isDead() const { return m_health <= 0; }

    private:
        Game::Terrain* m_terrain;
        float m_x, m_y;
        float m_vx, m_vy;
        
        // Physics constants
        const float GRAVITY = 500.0f;
        const float MOVE_SPEED = 100.0f;
        const float JUMP_FORCE = -250.0f;

        bool m_isGrounded;
        int m_health = 100;
    };
}
