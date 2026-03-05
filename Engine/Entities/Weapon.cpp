#include "Engine/Entities/Weapon.hpp"
#include <iostream>

namespace Engine {
namespace Entities {

    void Weapon::fire(Interfaces::IRaycaster* raycaster, Interfaces::IMap* map, 
                      glm::vec3 origin, glm::vec3 direction, 
                      std::vector<std::unique_ptr<Zombie>>& activeZombies) {
        
        if (!raycaster || !map) return;

        // Perform raycast
        auto result = raycaster->raycast(map, origin, direction, m_range);

        // Simple Zombie Hit detection
        float closestHitDist = result.hit ? glm::distance(origin, result.hitPoint) : m_range;
        
        for (auto& z : activeZombies) {
            glm::vec3 zMin = z->getPosition() - glm::vec3(0.5f, 0.0f, 0.5f);
            glm::vec3 zMax = z->getPosition() + glm::vec3(0.5f, 2.0f, 0.5f);

            // Basic ray-AABB test
            float tmin = (zMin.x - origin.x) / direction.x; 
            float tmax = (zMax.x - origin.x) / direction.x; 

            if (tmin > tmax) std::swap(tmin, tmax); 

            float tymin = (zMin.y - origin.y) / direction.y; 
            float tymax = (zMax.y - origin.y) / direction.y; 

            if (tymin > tymax) std::swap(tymin, tymax); 

            if ((tmin > tymax) || (tymin > tmax)) continue; 

            if (tymin > tmin) tmin = tymin; 
            if (tymax < tmax) tmax = tymax; 

            float tzmin = (zMin.z - origin.z) / direction.z; 
            float tzmax = (zMax.z - origin.z) / direction.z; 

            if (tzmin > tzmax) std::swap(tzmin, tzmax); 

            if ((tmin > tzmax) || (tzmin > tmax)) continue; 

            if (tzmin > tmin) tmin = tzmin; 
            if (tzmax < tmax) tmax = tzmax; 

            if (tmin > 0.0f && tmin < closestHitDist) {
                closestHitDist = tmin;
                std::cout << "ZOMBIE HIT!" << std::endl;
                // Deal damage z->takeDamage(...)
            }
        }
    }

}
}
