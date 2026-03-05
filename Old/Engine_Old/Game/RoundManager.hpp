#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Engine/Entities/Zombie.hpp"
#include "Engine/Game/VoxelMap.hpp"
#include "Engine/Entities/Player.hpp"

namespace Game {
    class RoundManager {
    public:
        RoundManager();

        void update(float deltaTime, std::vector<std::unique_ptr<Entities::Zombie>>& zombies, Game::VoxelMap* map, Entities::Player* player);
        
        int getCurrentRound() const { return m_currentRound; }
        int getZombiesRemaining() const { return m_zombiesRemainingToSpawn + m_activeZombies; }
        bool isRoundTransitioning() const { return m_roundTransitionTimer > 0.0f; }

    private:
        int m_currentRound;
        int m_zombiesRemainingToSpawn;
        int m_activeZombies;
        
        float m_spawnTimer;
        float m_roundTransitionTimer;

        void startNextRound();
        void spawnZombie(std::vector<std::unique_ptr<Entities::Zombie>>& zombies, Game::VoxelMap* map, Entities::Player* player);
        int calculateZombieHealth(int round);
        int calculateMaxZombies(int round);
    };
}
