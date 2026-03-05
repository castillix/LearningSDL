#include "Engine/Entities/Barrier.hpp"
#include "Engine/Graphics/Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace Entities {
    Barrier::Barrier(glm::vec3 position, int maxHealth) 
        : m_position(position), m_health(maxHealth), m_maxHealth(maxHealth), m_VAO(0), m_VBO(0), m_repairCooldown(0.0f) {
        initRender();
    }

    Barrier::~Barrier() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    Physics::AABB Barrier::getAABB() const {
        return Physics::AABB(m_position - glm::vec3(0.5f, 0.0f, 0.5f), m_position + glm::vec3(0.5f, 2.0f, 0.5f));
    }

    void Barrier::takeDamage(int amount) {
        if (!isBroken()) {
            m_health -= amount;
        }
    }

    bool Barrier::repair() {
        if (m_health < m_maxHealth && m_repairCooldown <= 0.0f) {
            m_health += 10;
            if (m_health > m_maxHealth) m_health = m_maxHealth;
            m_repairCooldown = 0.5f; // Can only repair a chunk every 0.5 seconds
            return true;
        }
        return false;
    }

    void Barrier::update(float deltaTime) {
        if (m_repairCooldown > 0.0f) {
            m_repairCooldown -= deltaTime;
        }
    }

    void Barrier::initRender() {
        // A simple flat wide board shape
        float vertices[] = {
            // Front
            -0.5f, 0.0f,  0.1f,   0.5f, 0.0f,  0.1f,   0.5f, 2.0f,  0.1f,
             0.5f, 2.0f,  0.1f,  -0.5f, 2.0f,  0.1f,  -0.5f, 0.0f,  0.1f,
            // Back
            -0.5f, 0.0f, -0.1f,   0.5f, 0.0f, -0.1f,   0.5f, 2.0f, -0.1f,
             0.5f, 2.0f, -0.1f,  -0.5f, 2.0f, -0.1f,  -0.5f, 0.0f, -0.1f,
            // Left
            -0.5f, 0.0f, -0.1f,  -0.5f, 0.0f,  0.1f,  -0.5f, 2.0f,  0.1f,
            -0.5f, 2.0f,  0.1f,  -0.5f, 2.0f, -0.1f,  -0.5f, 0.0f, -0.1f,
            // Right
             0.5f, 0.0f, -0.1f,   0.5f, 0.0f,  0.1f,   0.5f, 2.0f,  0.1f,
             0.5f, 2.0f,  0.1f,   0.5f, 2.0f, -0.1f,   0.5f, 0.0f, -0.1f,
            // Top
             0.5f, 2.0f, -0.1f,   0.5f, 2.0f,  0.1f,  -0.5f, 2.0f,  0.1f,
            -0.5f, 2.0f,  0.1f,  -0.5f, 2.0f, -0.1f,   0.5f, 2.0f, -0.1f,
            // Bottom
             0.5f, 0.0f, -0.1f,   0.5f, 0.0f,  0.1f,  -0.5f, 0.0f,  0.1f,
            -0.5f, 0.0f,  0.1f,  -0.5f, 0.0f, -0.1f,   0.5f, 0.0f, -0.1f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void Barrier::render(Graphics::Shader& shader) {
        // If broken, don't render or render flat
        if (isBroken()) return;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);

        // Scale based on health
        float hpPercent = (float)m_health / (float)m_maxHealth;
        model = glm::scale(model, glm::vec3(1.0f, hpPercent, 1.0f));

        shader.setMat4("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
