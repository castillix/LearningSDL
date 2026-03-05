#include "Worm.hpp"
#include <SDL3/SDL.h>

namespace Entities {
    Worm::Worm(Game::Terrain* terrain, float startX, float startY) 
        : m_terrain(terrain), m_x(startX), m_y(startY), m_vx(0.0f), m_vy(0.0f), m_isGrounded(false), m_health(100) {}

    void Worm::takeDamage(int amount) {
        m_health -= amount;
        if (m_health <= 0) {
            m_health = 0;
            destroy();
        }
    }

    void Worm::update(float deltaTime, const Core::InputManager& input) {
        // Horizontal Movement
        m_vx = 0.0f;
        if (input.isKeyHeld(SDL_SCANCODE_A) || input.isKeyHeld(SDL_SCANCODE_LEFT)) {
            m_vx = -MOVE_SPEED;
        } else if (input.isKeyHeld(SDL_SCANCODE_D) || input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
            m_vx = MOVE_SPEED;
        }

        // Jumping
        if (m_isGrounded && input.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
            m_vy = JUMP_FORCE;
            m_isGrounded = false;
        }

        // Propose Horizontal
        float nextX = m_x + m_vx * deltaTime;
        
        if (m_terrain) {
            if (m_terrain->isSolid(static_cast<int>(nextX), static_cast<int>(m_y))) {
                // Try climbing a slope
                bool climbed = false;
                for (int climb = 1; climb <= 5; ++climb) {
                    if (!m_terrain->isSolid(static_cast<int>(nextX), static_cast<int>(m_y - climb))) {
                        m_y -= climb;
                        m_x = nextX;
                        climbed = true;
                        break;
                    }
                }
                if (!climbed) {
                    m_vx = 0.0f; // Wall too steep
                }
            } else {
                m_x = nextX;
            }
        }

        // Apply Gravity
        m_vy += GRAVITY * deltaTime;
        float nextY = m_y + m_vy * deltaTime;

        // Vertical Collision
        if (m_terrain) {
            if (m_vy > 0.0f) { // Falling down
                bool hitGround = false;
                for (int y = static_cast<int>(m_y); y <= static_cast<int>(nextY); ++y) {
                    if (m_terrain->isSolid(static_cast<int>(m_x), y)) {
                        m_y = static_cast<float>(y - 1); // rest exactly on top of the solid pixel
                        m_vy = 0.0f;
                        m_isGrounded = true;
                        hitGround = true;
                        break;
                    }
                }
                if (!hitGround) {
                    m_y = nextY;
                    m_isGrounded = false;
                }
            } else if (m_vy < 0.0f) { // Jumping up
                bool hitCeiling = false;
                for (int y = static_cast<int>(m_y); y >= static_cast<int>(nextY); --y) {
                    if (m_terrain->isSolid(static_cast<int>(m_x), y - 10)) { // head collision
                        m_y = static_cast<float>(y); 
                        m_vy = 0.0f;
                        hitCeiling = true;
                        break;
                    }
                }
                if (!hitCeiling) {
                    m_y = nextY;
                }
            }
        }

        // Keep inside bounds
        if (m_x < 5.0f) m_x = 5.0f;
        if (m_x > m_terrain->getWidth() - 5.0f) m_x = static_cast<float>(m_terrain->getWidth() - 5);
        if (m_y > m_terrain->getHeight()) {
            destroy();
        }
    }

    void Worm::render(Graphics::Renderer* renderer) {
        // Draw the worm as a placeholder pink rectangle (10x10)
        // m_x, m_y represents bottom center of the worm
        SDL_FRect rect;
        rect.w = 10.0f;
        rect.h = 10.0f;
        rect.x = m_x - 5.0f;
        rect.y = m_y - 10.0f;

        renderer->setDrawColor(255, 105, 180, 255); // Hot pink
        renderer->fillRect(rect);
        
        // Draw an eye
        SDL_FRect eye = { m_x + 2.0f, m_y - 8.0f, 2.0f, 2.0f };
        if (m_vx < 0.0f) {
            eye.x = m_x - 4.0f; // Eye looks left
        }
        renderer->setDrawColor(0, 0, 0, 255);
        renderer->fillRect(eye);

        // Draw Health Bar
        if (m_health > 0) {
            SDL_FRect hbBackground = { m_x - 10.0f, m_y - 20.0f, 20.0f, 4.0f };
            renderer->setDrawColor(100, 100, 100, 255);
            renderer->fillRect(hbBackground);
            
            float healthRatio = static_cast<float>(m_health) / 100.0f;
            SDL_FRect hbForeground = { m_x - 10.0f, m_y - 20.0f, 20.0f * healthRatio, 4.0f };
            renderer->setDrawColor(0, 255, 0, 255); // Green health
            if (healthRatio < 0.5f) renderer->setDrawColor(255, 255, 0, 255); // Yellow
            if (healthRatio < 0.25f) renderer->setDrawColor(255, 0, 0, 255); // Red
            renderer->fillRect(hbForeground);
        }
    }
}
