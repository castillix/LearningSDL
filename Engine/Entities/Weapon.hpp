#pragma once
#include "Engine/Interfaces/IRaycaster.hpp"
#include "Engine/Entities/Zombie.hpp"
#include <vector>
#include <memory>

namespace Engine {
namespace Entities {

    class Weapon {
    public:
        Weapon() = default;

        void fire(Interfaces::IRaycaster* raycaster, Interfaces::IMap* map, 
                  glm::vec3 origin, glm::vec3 direction, 
                  std::vector<std::unique_ptr<Zombie>>& activeZombies);
        
    private:
        // Basic debug weapon variables
        float m_range = 100.0f;
        int m_damage = 50;
    };

}
}
