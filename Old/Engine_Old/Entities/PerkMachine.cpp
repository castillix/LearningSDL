#include "Engine/Entities/PerkMachine.hpp"
#include "Engine/Graphics/Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Entities {
    PerkMachine::PerkMachine(glm::vec3 position, PerkType type, int cost) 
        : m_position(position), m_type(type), m_cost(cost), m_VAO(0), m_VBO(0) {
        initRender();
    }

    PerkMachine::~PerkMachine() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    Physics::AABB PerkMachine::getAABB() const {
        return Physics::AABB(m_position - glm::vec3(0.5f, 0.0f, 0.5f), m_position + glm::vec3(0.5f, 2.0f, 0.5f));
    }
    
    std::string PerkMachine::getName() const {
        switch(m_type) {
            case PerkType::JUGGERNOG: return "Juggernog";
            case PerkType::QUICK_REVIVE: return "Quick Revive";
            case PerkType::SPEED_COLA: return "Speed Cola";
            case PerkType::DOUBLE_TAP: return "Double Tap";
            default: return "Unknown Perk";
        }
    }

    bool PerkMachine::interact(Entities::Player* player) {
        if (player->spendPoints(m_cost)) {
            switch (m_type) {
                case PerkType::JUGGERNOG:
                    // Hack for simplicity, real game would have a Player::hasPerk(PerkType) map
                    // Since it's a demonstration, increase max health and heal to full
                    player->takeDamage(-150); // Just give them huge health
                    SDL_Log("Bought Juggernog!");
                    break;
                case PerkType::QUICK_REVIVE:
                    SDL_Log("Bought Quick Revive!");
                    break;
                case PerkType::SPEED_COLA:
                    SDL_Log("Bought Speed Cola!");
                    break;
                case PerkType::DOUBLE_TAP:
                    SDL_Log("Bought Double Tap!");
                    break;
            }
            return true;
        }
        return false;
    }

    void PerkMachine::initRender() {
        // A tall rectangle shape representing the machine
        float vertices[] = {
            // Front
            -0.5f, 0.0f,  0.5f,   0.5f, 0.0f,  0.5f,   0.5f, 2.0f,  0.5f,
             0.5f, 2.0f,  0.5f,  -0.5f, 2.0f,  0.5f,  -0.5f, 0.0f,  0.5f,
            // Back
            -0.5f, 0.0f, -0.5f,   0.5f, 0.0f, -0.5f,   0.5f, 2.0f, -0.5f,
             0.5f, 2.0f, -0.5f,  -0.5f, 2.0f, -0.5f,  -0.5f, 0.0f, -0.5f,
            // Left
            -0.5f, 0.0f, -0.5f,  -0.5f, 0.0f,  0.5f,  -0.5f, 2.0f,  0.5f,
            -0.5f, 2.0f,  0.5f,  -0.5f, 2.0f, -0.5f,  -0.5f, 0.0f, -0.5f,
            // Right
             0.5f, 0.0f, -0.5f,   0.5f, 0.0f,  0.5f,   0.5f, 2.0f,  0.5f,
             0.5f, 2.0f,  0.5f,   0.5f, 2.0f, -0.5f,   0.5f, 0.0f, -0.5f,
            // Top
             0.5f, 2.0f, -0.5f,   0.5f, 2.0f,  0.5f,  -0.5f, 2.0f,  0.5f,
            -0.5f, 2.0f,  0.5f,  -0.5f, 2.0f, -0.5f,   0.5f, 2.0f, -0.5f,
            // Bottom
             0.5f, 0.0f, -0.5f,   0.5f, 0.0f,  0.5f,  -0.5f, 0.0f,  0.5f,
            -0.5f, 0.0f,  0.5f,  -0.5f, 0.0f, -0.5f,   0.5f, 0.0f, -0.5f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void PerkMachine::render(Graphics::Shader& shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        shader.setMat4("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
