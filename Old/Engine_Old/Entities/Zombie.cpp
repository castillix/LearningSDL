#include "Engine/Entities/Zombie.hpp"
#include "Engine/Graphics/Shader.hpp"
#include <glad/glad.h>
#include <algorithm>
#include <cmath>

namespace Entities {
    Zombie::Zombie(glm::vec3 startPos, int startHealth) 
        : m_position(startPos), m_velocity(0.0f), m_health(startHealth), m_isGrounded(false), m_VAO(0), m_VBO(0) {
        initRender();
    }

    Zombie::~Zombie() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    Physics::AABB Zombie::getAABB() const {
        return Physics::AABB(m_position - glm::vec3(0.3f, 0.0f, 0.3f), m_position + glm::vec3(0.3f, 1.8f, 0.3f));
    }

    void Zombie::takeDamage(int amount) {
        m_health -= amount;
    }

    // Helper for AABB intersection
    bool checkAABBIntersect(const Physics::AABB& a, const Physics::AABB& b) {
        return (a.minExtents.x <= b.maxExtents.x && a.maxExtents.x >= b.minExtents.x) &&
               (a.minExtents.y <= b.maxExtents.y && a.maxExtents.y >= b.minExtents.y) &&
               (a.minExtents.z <= b.maxExtents.z && a.maxExtents.z >= b.minExtents.z);
    }

    void Zombie::update(float deltaTime, Game::VoxelMap* map, AI::FlowField* flowField, std::vector<std::unique_ptr<Entities::Barrier>>& barriers) {
        if (isDead()) return;

        glm::vec3 dir(0.0f);
        if (flowField) {
            dir = flowField->getDirection(m_position);
            dir.y = 0; // Don't fly towards the player
        }

        glm::vec3 accel(0.0f);
        if (glm::length(dir) > 0.1f) {
            dir = glm::normalize(dir);
            accel = dir * 10.0f; // Speed
        }

        // Apply friction
        m_velocity.x *= 0.8f; 
        m_velocity.z *= 0.8f; 

        m_velocity.x += accel.x * deltaTime;
        m_velocity.z += accel.z * deltaTime;

        // Apply Gravity
        m_velocity.y -= 30.0f * deltaTime;
        
        m_position += m_velocity * deltaTime;

        // Check barrier collisions
        bool attacking = false;
        Physics::AABB zBox = getAABB();
        for (auto& b : barriers) {
            if (!b->isBroken() && checkAABBIntersect(zBox, b->getAABB())) {
                // Colliding with barrier, revert movement and attack
                m_position -= m_velocity * deltaTime; // Stop
                b->takeDamage(1); // Attack 1 hp per frame (very fast)
                attacking = true;
                break;
            }
        }

        if (!attacking) {
            solveCollision(map);
        }
    }

    void Zombie::solveCollision(Game::VoxelMap* map) {
        if (!map) return;
        m_isGrounded = false;
        
        Physics::AABB zBox = getAABB();

        int minX = (int)std::floor(m_position.x - 0.5f);
        int maxX = (int)std::ceil(m_position.x + 0.5f);
        int minY = (int)std::floor(m_position.y - 0.5f);
        int maxY = (int)std::ceil(m_position.y + 2.0f);
        int minZ = (int)std::floor(m_position.z - 0.5f);
        int maxZ = (int)std::ceil(m_position.z + 0.5f);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    if (map->getBlock(x, y, z) != 0) {
                        Physics::AABB blockBox(glm::vec3(x, y, z), glm::vec3(x + 1, y + 1, z + 1));
                        if (zBox.intersects(blockBox)) {
                            // Resolve
                            float dx1 = blockBox.maxExtents.x - zBox.minExtents.x;
                            float dx2 = zBox.maxExtents.x - blockBox.minExtents.x;
                            float dy1 = blockBox.maxExtents.y - zBox.minExtents.y;
                            float dy2 = zBox.maxExtents.y - blockBox.minExtents.y;
                            float dz1 = blockBox.maxExtents.z - zBox.minExtents.z;
                            float dz2 = zBox.maxExtents.z - blockBox.minExtents.z;

                            float penX = std::min(dx1, dx2);
                            float penY = std::min(dy1, dy2);
                            float penZ = std::min(dz1, dz2);

                            if (penX < penY && penX < penZ) {
                                if (dx1 < dx2) m_position.x += penX; else m_position.x -= penX;
                                m_velocity.x = 0;
                            } else if (penY < penX && penY < penZ) {
                                if (dy1 < dy2) {
                                    m_position.y += penY;
                                    m_isGrounded = true;
                                    if (m_velocity.y < 0) m_velocity.y = 0;
                                } else {
                                    m_position.y -= penY;
                                    if (m_velocity.y > 0) m_velocity.y = 0;
                                }
                            } else {
                                if (dz1 < dz2) m_position.z += penZ; else m_position.z -= penZ;
                                m_velocity.z = 0;
                            }
                            zBox = getAABB();
                        }
                    }
                }
            }
        }
    }

    void Zombie::initRender() {
        float vertices[] = {
            // Front
            -0.3f, 0.0f,  0.3f, 4.0f, 0.0f,   0.3f, 0.0f,  0.3f, 4.0f, 0.0f,   0.3f, 1.8f,  0.3f, 4.0f, 0.0f,
             0.3f, 1.8f,  0.3f, 4.0f, 0.0f,  -0.3f, 1.8f,  0.3f, 4.0f, 0.0f,  -0.3f, 0.0f,  0.3f, 4.0f, 0.0f,
            // Back
            -0.3f, 0.0f, -0.3f, 4.0f, 0.0f,   0.3f, 0.0f, -0.3f, 4.0f, 0.0f,   0.3f, 1.8f, -0.3f, 4.0f, 0.0f,
             0.3f, 1.8f, -0.3f, 4.0f, 0.0f,  -0.3f, 1.8f, -0.3f, 4.0f, 0.0f,  -0.3f, 0.0f, -0.3f, 4.0f, 0.0f,
            // Left
            -0.3f, 0.0f, -0.3f, 4.0f, 0.0f,  -0.3f, 0.0f,  0.3f, 4.0f, 0.0f,  -0.3f, 1.8f,  0.3f, 4.0f, 0.0f,
            -0.3f, 1.8f,  0.3f, 4.0f, 0.0f,  -0.3f, 1.8f, -0.3f, 4.0f, 0.0f,  -0.3f, 0.0f, -0.3f, 4.0f, 0.0f,
            // Right
             0.3f, 0.0f, -0.3f, 4.0f, 0.0f,   0.3f, 0.0f,  0.3f, 4.0f, 0.0f,   0.3f, 1.8f,  0.3f, 4.0f, 0.0f,
             0.3f, 1.8f,  0.3f, 4.0f, 0.0f,   0.3f, 1.8f, -0.3f, 4.0f, 0.0f,   0.3f, 0.0f, -0.3f, 4.0f, 0.0f,
            // Top
             0.3f, 1.8f, -0.3f, 4.0f, 0.0f,   0.3f, 1.8f,  0.3f, 4.0f, 0.0f,  -0.3f, 1.8f,  0.3f, 4.0f, 0.0f,
            -0.3f, 1.8f,  0.3f, 4.0f, 0.0f,  -0.3f, 1.8f, -0.3f, 4.0f, 0.0f,   0.3f, 1.8f, -0.3f, 4.0f, 0.0f,
            // Bottom
             0.3f, 0.0f, -0.3f, 4.0f, 0.0f,   0.3f, 0.0f,  0.3f, 4.0f, 0.0f,  -0.3f, 0.0f,  0.3f, 4.0f, 0.0f,
            -0.3f, 0.0f,  0.3f, 4.0f, 0.0f,  -0.3f, 0.0f, -0.3f, 4.0f, 0.0f,   0.3f, 0.0f, -0.3f, 4.0f, 0.0f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void Zombie::render(Graphics::Shader& shader) {
        if (isDead()) return;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        shader.setMat4("model", model);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
