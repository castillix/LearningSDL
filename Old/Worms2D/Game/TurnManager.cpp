#include "TurnManager.hpp"
#include <SDL3/SDL.h>
#include <cmath>

namespace Game {
    void TurnManager::addTeam(const std::string& name) {
        Team t;
        t.name = name;
        m_teams.push_back(t);
    }

    void TurnManager::addWormToTeam(int teamIndex, Entities::Worm* worm) {
        if (teamIndex >= 0 && teamIndex < m_teams.size()) {
            m_teams[teamIndex].worms.push_back(worm);
        }
    }

    void TurnManager::startMatch() {
        if (m_teams.empty()) return;
        m_currentTeamIndex = 0;
        m_turnTimer = MAX_TURN_TIME;
        SDL_Log("Match Started! It is %s's turn.", m_teams[m_currentTeamIndex].name.c_str());
    }

    void TurnManager::update(float deltaTime) {
        if (m_teams.empty()) return;

        m_turnTimer -= deltaTime;
        if (m_turnTimer <= 0.0f) {
            endTurn();
        }
    }

    void TurnManager::applyExplosionDamage(float ex, float ey, int radius) {
        float r2 = radius * radius;
        for (auto& team : m_teams) {
            for (auto* worm : team.worms) {
                if (worm->isDead()) continue;

                float dx = worm->getX() - ex;
                float dy = worm->getY() - ey;
                float dist2 = dx * dx + dy * dy;

                if (dist2 <= r2) {
                    // Simple linear distance falloff out to the explosion radius
                    float dist = std::sqrt(dist2);
                    float damageScale = 1.0f - (dist / radius);
                    int damage = static_cast<int>(50.0f * damageScale); // 50 max damage
                    
                    if (damage > 0) {
                        worm->takeDamage(damage);
                        SDL_Log("Worm took %d damage! HP: %d", damage, worm->getHealth());
                    }
                }
            }
        }
    }

    void TurnManager::endTurn() {
        if (m_teams.empty()) return;

        // Advance to next worm in the CURRENT team before switching teams
        Team& currentTeam = m_teams[m_currentTeamIndex];
        if (!currentTeam.worms.empty()) {
            currentTeam.currentWormIndex = (currentTeam.currentWormIndex + 1) % currentTeam.worms.size();
        }

        // Switch to next team
        m_currentTeamIndex = (m_currentTeamIndex + 1) % m_teams.size();
        m_turnTimer = MAX_TURN_TIME;

        SDL_Log("Turn ended. It is now %s's turn.", m_teams[m_currentTeamIndex].name.c_str());
    }

    Entities::Worm* TurnManager::getActiveWorm() const {
        if (m_teams.empty()) return nullptr;
        const Team& currentTeam = m_teams[m_currentTeamIndex];
        if (currentTeam.worms.empty()) return nullptr;

        return currentTeam.worms[currentTeam.currentWormIndex];
    }

    std::string TurnManager::getActiveTeamName() const {
        if (m_teams.empty()) return "None";
        return m_teams[m_currentTeamIndex].name;
    }
}
