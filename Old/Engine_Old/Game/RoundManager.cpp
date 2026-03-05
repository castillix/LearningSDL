#include "Engine/Game/RoundManager.hpp"
#include <cstdlib>
#include <SDL3/SDL.h>

namespace Game {
    RoundManager::RoundManager() 
        : m_currentRound(0), m_zombiesRemainingToSpawn(0), m_activeZombies(0),
          m_spawnTimer(0.0f), m_roundTransitionTimer(5.0f) {
    }

    int RoundManager::calculateZombieHealth(int round) {
        if (round < 10) return 100 + (round * 50);
        return (int)(100 * std::pow(1.1f, round - 9)); // CoD style scaling
    }

    int RoundManager::calculateMaxZombies(int round) {
        return 6 + (int)(round * 2.5f); // Rough estimate
    }

    void RoundManager::startNextRound() {
        m_currentRound++;
        m_zombiesRemainingToSpawn = calculateMaxZombies(m_currentRound);
        m_roundTransitionTimer = 0.0f; // Done transitioning
        SDL_Log("Starting Round %d! Zombies: %d, Health: %d", m_currentRound, m_zombiesRemainingToSpawn, calculateZombieHealth(m_currentRound));
    }

    void RoundManager::spawnZombie(std::vector<std::unique_ptr<Entities::Zombie>>& zombies, Game::VoxelMap* map, Entities::Player* player) {
        // Spawn logic: find a valid spawn point. For now, a semi-random location near edge of map
        int spawnX = (rand() % 60) + 2; // Arbitrary map bounds 2 to 62
        int spawnZ = (rand() % 60) + 2;

        int health = calculateZombieHealth(m_currentRound);
        zombies.push_back(std::make_unique<Entities::Zombie>(glm::vec3(spawnX, 4.0f, spawnZ), health));
        m_zombiesRemainingToSpawn--;
        m_activeZombies++;
    }

    void RoundManager::update(float deltaTime, std::vector<std::unique_ptr<Entities::Zombie>>& zombies, Game::VoxelMap* map, Entities::Player* player) {
        // Clear dead zombies and count active
        m_activeZombies = 0;
        for (auto it = zombies.begin(); it != zombies.end();) {
            if ((*it)->isDead()) {
                player->addPoints(130); // Points for a kill
                it = zombies.erase(it);
            } else {
                m_activeZombies++;
                ++it;
            }
        }

        if (m_roundTransitionTimer > 0.0f) {
            m_roundTransitionTimer -= deltaTime;
            if (m_roundTransitionTimer <= 0.0f) {
                startNextRound();
            }
            return;
        }

        // Spawn director logic
        if (m_zombiesRemainingToSpawn > 0 && m_activeZombies < 24) { // Max 24 zombies at once
            m_spawnTimer -= deltaTime;
            if (m_spawnTimer <= 0.0f) {
                spawnZombie(zombies, map, player);
                m_spawnTimer = 2.0f - (m_currentRound * 0.1f); // Spawn faster in higher rounds
                if (m_spawnTimer < 0.2f) m_spawnTimer = 0.2f;
            }
        }

        // Round completion check
        if (m_zombiesRemainingToSpawn == 0 && m_activeZombies == 0 && m_roundTransitionTimer <= 0.0f) {
            m_roundTransitionTimer = 10.0f; // 10 second break between rounds
            SDL_Log("Round cleared! Next round in 10 seconds.");
        }
    }
}
