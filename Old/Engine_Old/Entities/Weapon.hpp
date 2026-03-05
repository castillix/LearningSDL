#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Engine/Entities/Zombie.hpp"
#include "Engine/Physics/Raycaster.hpp"
#include <vector>

namespace Entities {
    class Weapon {
    public:
        Weapon(std::string name, int damage, int magSize, int ammoReserve, float fireRate, float reloadTime);

        void fire(const glm::vec3& origin, const glm::vec3& dir, Game::VoxelMap* map, std::vector<std::unique_ptr<Entities::Zombie>>& zombies);
        void reload();
        void update(float deltaTime);

        bool isReloading() const { return m_reloadTimer > 0.0f; }
        
        std::string getName() const { return m_name; }
        int getAmmoInMag() const { return m_ammoInMag; }
        int getAmmoReserve() const { return m_ammoReserve; }

    private:
        std::string m_name;
        int m_damage;
        
        int m_magSize;
        int m_ammoInMag;
        int m_ammoReserve;
        
        float m_fireRate; // Time between shots in seconds
        float m_reloadTime;

        float m_fireTimer;
        float m_reloadTimer;

        bool rayIntersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const Physics::AABB& box, float& tHit);
    };
}
