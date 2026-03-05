#pragma once
#include <glm/glm.hpp>
#include "Engine/Physics/AABB.hpp"
#include "Engine/Game/VoxelMap.hpp"
#include "Engine/Graphics/Camera3D.hpp"
#include "Engine/Graphics/Shader.hpp"
#include "Engine/AI/FlowField.hpp"
#include "Engine/Entities/Barrier.hpp"
#include <vector>
#include <memory>

namespace Entities {
    class Zombie {
    public:
        Zombie(glm::vec3 startPos, int startHealth);
        ~Zombie();

        void update(float deltaTime, Game::VoxelMap* map, AI::FlowField* flowField, std::vector<std::unique_ptr<Entities::Barrier>>& barriers);
        void render(Graphics::Shader& shader);
        void takeDamage(int amount);

        bool isDead() const { return m_health <= 0; }
        
        glm::vec3 getPosition() const { return m_position; }
        Physics::AABB getAABB() const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_velocity;
        int m_health;
        bool m_isGrounded;

        unsigned int m_VAO, m_VBO;
        void initRender();

        void solveCollision(Game::VoxelMap* map);
    };
}
