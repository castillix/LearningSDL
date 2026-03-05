#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Engine/Physics/AABB.hpp"
#include "Engine/Entities/Player.hpp"

namespace Graphics { class Shader; }

namespace Entities {
    enum class PerkType {
        JUGGERNOG, 
        QUICK_REVIVE,
        SPEED_COLA,
        DOUBLE_TAP
    };

    class PerkMachine {
    public:
        PerkMachine(glm::vec3 position, PerkType type, int cost);
        ~PerkMachine();

        void render(Graphics::Shader& shader);
        bool interact(Entities::Player* player);

        Physics::AABB getAABB() const;
        glm::vec3 getPosition() const { return m_position; }
        std::string getName() const;

    private:
        glm::vec3 m_position;
        PerkType m_type;
        int m_cost;

        unsigned int m_VAO, m_VBO;
        void initRender();
    };
}
