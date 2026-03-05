#pragma once
#include <glm/glm.hpp>
#include "Engine/Physics/AABB.hpp"

namespace Graphics { class Shader; }

namespace Entities {
    class Barrier {
    public:
        Barrier(glm::vec3 position, int maxHealth);
        ~Barrier();

        void update(float deltaTime);
        void render(Graphics::Shader& shader);
        
        bool repair();
        void takeDamage(int amount);

        bool isBroken() const { return m_health <= 0; }
        
        Physics::AABB getAABB() const;
        glm::vec3 getPosition() const { return m_position; }

    private:
        glm::vec3 m_position;
        int m_health;
        int m_maxHealth;

        unsigned int m_VAO, m_VBO;
        void initRender();
        float m_repairCooldown;
    };
}
