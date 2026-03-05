#pragma once
#include "Engine/Interfaces/IEntity.hpp"
#include "Engine/Interfaces/IPathfinder.hpp"
#include <glm/glm.hpp>

namespace Engine {
namespace Entities {

    class Zombie : public Interfaces::IEntity {
    public:
        Zombie(glm::vec3 spawnPos);
        ~Zombie() override = default;

        void update(float deltaTime, class Interfaces::IMap* map) override;
        void render(class Interfaces::IRenderer* renderer) override;

        glm::vec3 getPosition() const { return m_position; }
        
        void navigate(class Engine::Interfaces::IPathfinder* pathfinder, float deltaTime);

    private:
        glm::vec3 m_position;
        glm::vec3 m_velocity;
        float m_speed;
        int m_health;
        
        unsigned int m_vao, m_vbo;
        void initMesh();

        // Minimal rendering variables (No large mesh generation for now, just placeholder box data)
        void solveCollision(class Interfaces::IMap* map);
    };

}
}
