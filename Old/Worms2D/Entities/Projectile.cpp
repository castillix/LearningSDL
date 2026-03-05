#include "Projectile.hpp"
#include <SDL3/SDL.h>

namespace Entities {
    Projectile::Projectile(Game::Terrain* terrain, float startX, float startY, float vx, float vy, ExplodeCallback onExplode)
        : m_terrain(terrain), m_x(startX), m_y(startY), m_vx(vx), m_vy(vy), m_onExplode(onExplode) {}

    void Projectile::update(float deltaTime, const Core::InputManager& input) {
        // Apply wind (omitted for now)
        
        // Apply Gravity
        m_vy += GRAVITY * deltaTime;

        // Propose new positions
        float nextX = m_x + m_vx * deltaTime;
        float nextY = m_y + m_vy * deltaTime;

        // Check bounds
        if (nextX < 0 || nextX >= m_terrain->getWidth() || nextY >= m_terrain->getHeight()) {
            destroy(); // Fell out of bounds
            return;
        }

        // Raycast-style check
        if (m_terrain->isSolid(static_cast<int>(nextX), static_cast<int>(nextY))) {
            // BOOM!
            m_terrain->destroyCircle(static_cast<int>(nextX), static_cast<int>(nextY), EXPLOSION_RADIUS);
            
            if (m_onExplode) {
                m_onExplode(nextX, nextY, EXPLOSION_RADIUS);
            }
            
            destroy();
            return;
        }

        m_x = nextX;
        m_y = nextY;
    }

    void Projectile::render(Graphics::Renderer* renderer) {
        SDL_FRect rect = { m_x - 3.0f, m_y - 3.0f, 6.0f, 6.0f };
        renderer->setDrawColor(200, 50, 50, 255); // Red bomb
        renderer->fillRect(rect);
    }
}
