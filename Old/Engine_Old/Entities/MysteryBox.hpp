#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Engine/Physics/AABB.hpp"
#include "Engine/Entities/Weapon.hpp"
#include "Engine/Entities/Player.hpp"

namespace Graphics { class Shader; }

namespace Entities {
    class MysteryBox {
    public:
        MysteryBox(glm::vec3 position);
        ~MysteryBox();

        void update(float deltaTime);
        void render(Graphics::Shader& shader);
        
        bool interact(Entities::Player* player);

        Physics::AABB getAABB() const;
        glm::vec3 getPosition() const { return m_position; }

    private:
        glm::vec3 m_position;

        unsigned int m_VAO, m_VBO;
        void initRender();

        bool m_isSpinning;
        float m_spinTimer;
        
        // Weapon pool
        std::vector<Weapon> m_weaponPool;
    };
}
