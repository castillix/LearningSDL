#pragma once
#include <vector>
#include <string>
#include "Engine/Entities/Worm.hpp"

namespace Game {
    struct Team {
        std::string name;
        std::vector<Entities::Worm*> worms;
        int currentWormIndex = 0;
    };

    class TurnManager {
    public:
        void addTeam(const std::string& name);
        void addWormToTeam(int teamIndex, Entities::Worm* worm);

        void startMatch();
        void update(float deltaTime);
        void applyExplosionDamage(float ex, float ey, int radius);

        Entities::Worm* getActiveWorm() const;
        float getTurnTimeRemaining() const { return m_turnTimer; }
        std::string getActiveTeamName() const;

    private:
        void endTurn();

        std::vector<Team> m_teams;
        int m_currentTeamIndex = 0;
        
        float m_turnTimer = 0.0f;
        const float MAX_TURN_TIME = 45.0f; // 45 seconds per turn
    };
}
