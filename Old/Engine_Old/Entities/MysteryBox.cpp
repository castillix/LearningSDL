#include "Engine/Entities/MysteryBox.hpp"
#include "Engine/Graphics/Shader.hpp"
#include <glad/glad.h>
#include <cstdlib>

namespace Entities {
    MysteryBox::MysteryBox(glm::vec3 position) 
        : m_position(position), m_isSpinning(false), m_spinTimer(0.0f), m_VAO(0), m_VBO(0) {
        
        initRender();

        // Populate pool
        m_weaponPool.push_back(Weapon("Ray Gun", 1000, 20, 160, 0.2f, 2.0f));
        m_weaponPool.push_back(Weapon("MP40", 40, 32, 192, 0.11f, 2.4f));
        m_weaponPool.push_back(Weapon("Trench Gun", 200, 6, 60, 0.8f, 3.0f));
        m_weaponPool.push_back(Weapon("M16", 60, 30, 120, 0.1f, 2.0f));
        m_weaponPool.push_back(Weapon("Thundergun", 5000, 2, 14, 1.0f, 4.0f));
    }

    MysteryBox::~MysteryBox() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    Physics::AABB MysteryBox::getAABB() const {
        return Physics::AABB(m_position - glm::vec3(1.0f, 0.0f, 0.5f), m_position + glm::vec3(1.0f, 1.0f, 0.5f));
    }

    bool MysteryBox::interact(Entities::Player* player) {
        if (m_isSpinning) return false;

        if (player->spendPoints(950)) {
            m_isSpinning = true;
            m_spinTimer = 3.0f; // Spin for 3 seconds
            
            // Give weapon immediately, visual spin acts as cooldown
            int rIndex = rand() % m_weaponPool.size();
            player->giveWeapon(m_weaponPool[rIndex]);
            return true;
        }
        return false;
    }

    void MysteryBox::update(float deltaTime) {
        if (m_isSpinning) {
            m_spinTimer -= deltaTime;
            if (m_spinTimer <= 0.0f) {
                m_isSpinning = false;
            }
        }
    }

    void MysteryBox::initRender() {
        // A wider box shape representing the mystery box
        float vertices[] = {
            // Front
            -1.0f, 0.0f,  0.5f,   1.0f, 0.0f,  0.5f,   1.0f, 1.0f,  0.5f,
             1.0f, 1.0f,  0.5f,  -1.0f, 1.0f,  0.5f,  -1.0f, 0.0f,  0.5f,
            // Back
            -1.0f, 0.0f, -0.5f,   1.0f, 0.0f, -0.5f,   1.0f, 1.0f, -0.5f,
             1.0f, 1.0f, -0.5f,  -1.0f, 1.0f, -0.5f,  -1.0f, 0.0f, -0.5f,
            // Left
            -1.0f, 0.0f, -0.5f,  -1.0f, 0.0f,  0.5f,  -1.0f, 1.0f,  0.5f,
            -1.0f, 1.0f,  0.5f,  -1.0f, 1.0f, -0.5f,  -1.0f, 0.0f, -0.5f,
            // Right
             1.0f, 0.0f, -0.5f,   1.0f, 0.0f,  0.5f,   1.0f, 1.0f,  0.5f,
             1.0f, 1.0f,  0.5f,   1.0f, 1.0f, -0.5f,   1.0f, 0.0f, -0.5f,
            // Top
             1.0f, 1.0f, -0.5f,   1.0f, 1.0f,  0.5f,  -1.0f, 1.0f,  0.5f,
            -1.0f, 1.0f,  0.5f,  -1.0f, 1.0f, -0.5f,   1.0f, 1.0f, -0.5f,
            // Bottom
             1.0f, 0.0f, -0.5f,   1.0f, 0.0f,  0.5f,  -1.0f, 0.0f,  0.5f,
            -1.0f, 0.0f,  0.5f,  -1.0f, 0.0f, -0.5f,   1.0f, 0.0f, -0.5f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void MysteryBox::render(Graphics::Shader& shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        
        // If spinning, maybe bob it up and down slightly (visual feedback)
        if (m_isSpinning) {
            model = glm::translate(model, glm::vec3(0.0f, std::abs(std::sin(m_spinTimer * 10.0f)) * 0.2f, 0.0f));
        }

        shader.setMat4("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
