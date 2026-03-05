#include "WormsGame.hpp"
#include "Engine/Entities/Worm.hpp"
#include "Engine/Entities/Projectile.hpp"
#include <SDL3/SDL.h>
#include <cmath>

namespace Game {
    WormsGame::WormsGame() : m_terrain(800, 600) {}
    bool WormsGame::init(Graphics::Renderer* renderer, Graphics::TextureManager* textures, Audio::SoundManager* audio) {
        m_textures = textures;
        m_audio = audio;
        
        m_terrain.init(renderer);

        m_turnManager.addTeam("Red Team");
        m_turnManager.addTeam("Blue Team");

        auto w1 = std::make_unique<Entities::Worm>(&m_terrain, 200.0f, 50.0f);
        m_turnManager.addWormToTeam(0, w1.get());
        m_entityManager.addEntity(std::move(w1));

        auto w2 = std::make_unique<Entities::Worm>(&m_terrain, 600.0f, 50.0f);
        m_turnManager.addWormToTeam(1, w2.get());
        m_entityManager.addEntity(std::move(w2));

        m_turnManager.startMatch();

        // Setup initial game state here (load textures/sounds, spawn entities)
        SDL_Log("WormsGame Initialized!");
        return true;
    }

    void WormsGame::update(float deltaTime, const Core::InputManager& input) {
        // Handle global game logic here (turn transitions, camera movement)
        
        // Test terrain destruction
        if (input.isMouseJustPressed(SDL_BUTTON_LEFT)) {
            float mx, my;
            input.getMousePosition(mx, my);
            m_terrain.destroyCircle(static_cast<int>(mx), static_cast<int>(my), 40);
        }

        // Test firing projectile
        Entities::Worm* activeWorm = m_turnManager.getActiveWorm();
        if (activeWorm && input.isMouseJustPressed(SDL_BUTTON_RIGHT)) {
            float mx, my;
            input.getMousePosition(mx, my);
            
            float wx = activeWorm->getX();
            float wy = activeWorm->getY() - 5.0f; // Shoot from slightly above feet
            
            float dx = mx - wx;
            float dy = my - wy;
            
            // Normalize and apply power
            float length = std::sqrt(dx*dx + dy*dy);
            if (length > 0) {
                dx /= length;
                dy /= length;
                
                float power = 400.0f; // Fixed power for test
                auto onExplode = [this](float ex, float ey, int radius) {
                    this->m_turnManager.applyExplosionDamage(ex, ey, radius);
                };
                m_entityManager.addEntity(std::make_unique<Entities::Projectile>(&m_terrain, wx, wy, dx * power, dy * power, onExplode));
            }
        }

        m_turnManager.update(deltaTime);
        m_entityManager.update(deltaTime, input);
    }

    void WormsGame::render(Graphics::Renderer* renderer) {
        // Draw terrain here
        m_terrain.render(renderer);
        m_entityManager.render(renderer);

        // Draw HUD: Turn Indicator Box
        Entities::Worm* activeWorm = m_turnManager.getActiveWorm();
        if (activeWorm) {
            // Outline active worm
            SDL_FRect activeBox = { activeWorm->getX() - 7.0f, activeWorm->getY() - 12.0f, 14.0f, 14.0f };
            renderer->setDrawColor(255, 255, 255, 255);
            renderer->fillRect(activeBox); // Just a simple white square strictly beneath the actual worm to highlight them!

            // Turn time bar at top
            float timeRatio = m_turnManager.getTurnTimeRemaining() / 45.0f; // MAX_TURN_TIME
            SDL_FRect timeBarContainer = { 10.0f, 10.0f, 300.0f, 20.0f };
            renderer->setDrawColor(0, 0, 0, 200);
            renderer->fillRect(timeBarContainer);

            SDL_FRect timeBar = { 10.0f, 10.0f, 300.0f * timeRatio, 20.0f };
            renderer->setDrawColor(0, 150, 255, 255); // Blue turn bar
            renderer->fillRect(timeBar);
            
            // Note: Since SDL_TTF isn't imported, text rendering isn't available easily right now.
            // Using shapes for HUD.
        }
    }

    void WormsGame::cleanup() {
        m_entityManager.clear();
        SDL_Log("WormsGame Cleaned up!");
    }
}
