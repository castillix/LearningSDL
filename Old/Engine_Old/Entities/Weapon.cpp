#include "Engine/Entities/Weapon.hpp"
#include <SDL3/SDL.h>

namespace Entities {
    Weapon::Weapon(std::string name, int damage, int magSize, int ammoReserve, float fireRate, float reloadTime)
        : m_name(name), m_damage(damage), m_magSize(magSize), m_ammoInMag(magSize), 
          m_ammoReserve(ammoReserve), m_fireRate(fireRate), m_reloadTime(reloadTime), 
          m_fireTimer(0.0f), m_reloadTimer(0.0f) {}

    void Weapon::update(float deltaTime) {
        if (m_fireTimer > 0.0f) m_fireTimer -= deltaTime;
        
        if (m_reloadTimer > 0.0f) {
            m_reloadTimer -= deltaTime;
            if (m_reloadTimer <= 0.0f) {
                // Finish reload
                int needed = m_magSize - m_ammoInMag;
                if (m_ammoReserve >= needed) {
                    m_ammoInMag += needed;
                    m_ammoReserve -= needed;
                } else {
                    m_ammoInMag += m_ammoReserve;
                    m_ammoReserve = 0;
                }
            }
        }
    }

    void Weapon::reload() {
        if (m_ammoInMag < m_magSize && m_ammoReserve > 0 && m_reloadTimer <= 0.0f) {
            m_reloadTimer = m_reloadTime;
        }
    }

    // SLAB-method implementation for Ray vs AABB
    bool Weapon::rayIntersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const Physics::AABB& box, float& tHit) {
        glm::vec3 invDir = 1.0f / rayDir;
        glm::vec3 t0 = (box.minExtents - rayOrigin) * invDir;
        glm::vec3 t1 = (box.maxExtents - rayOrigin) * invDir;
        glm::vec3 tmin = glm::min(t0, t1);
        glm::vec3 tmax = glm::max(t0, t1);

        float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
        float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

        if (tNear > tFar || tFar < 0.0f) {
            return false;
        }

        tHit = tNear > 0.0f ? tNear : tFar;
        return true;
    }

    void Weapon::fire(const glm::vec3& origin, const glm::vec3& dir, Game::VoxelMap* map, std::vector<std::unique_ptr<Entities::Zombie>>& zombies) {
        if (m_fireTimer > 0.0f || m_reloadTimer > 0.0f || m_ammoInMag <= 0) {
            return;
        }

        m_ammoInMag--;
        m_fireTimer = m_fireRate;

        float maxRange = 100.0f;
        
        // Find closest map hit
        float closestHitDist = maxRange;
        Physics::RaycastResult mapRes;
        
        mapRes = Physics::Raycaster::raycast(map, origin, dir, maxRange);
        if (mapRes.hit) {
            closestHitDist = glm::distance(origin, mapRes.hitPos);
        }

        // Find closest zombie hit
        Entities::Zombie* hitZombie = nullptr;

        for (auto& z : zombies) {
            if (z->isDead()) continue;

            float tHit;
            if (rayIntersectAABB(origin, dir, z->getAABB(), tHit)) {
                if (tHit < closestHitDist) {
                    closestHitDist = tHit;
                    hitZombie = z.get();
                }
            }
        }

        if (hitZombie) {
            hitZombie->takeDamage(m_damage);
        }
    }
}
