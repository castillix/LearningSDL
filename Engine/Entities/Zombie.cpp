#include "Engine/Entities/Zombie.hpp"
#include "Engine/Interfaces/IMap.hpp"
#include "Engine/Interfaces/IPathfinder.hpp"
#include <algorithm>
#include <cmath>
#include <glad/glad.h>

namespace Engine {
namespace Entities {

    Zombie::Zombie(glm::vec3 spawnPos) 
        : m_position(spawnPos), m_velocity(0.0f), m_speed(2.0f), m_health(100) {
        initMesh();
    }

    void Zombie::navigate(Interfaces::IPathfinder* pathfinder, float deltaTime) {
        if (!pathfinder) return;
        
        glm::ivec3 currentPos(std::floor(m_position.x), std::floor(m_position.y), std::floor(m_position.z));
        glm::vec3 direction = pathfinder->getDirectionFrom(currentPos);
        
        if (glm::length(direction) > 0.0f) {
            m_velocity.x = direction.x * m_speed;
            m_velocity.z = direction.z * m_speed;
        } else {
            m_velocity.x = 0;
            m_velocity.z = 0;
        }
    }

    void Zombie::update(float deltaTime, Interfaces::IMap* map) {
        // Gravity
        m_velocity.y -= 30.0f * deltaTime;
        
        m_position += m_velocity * deltaTime;
        solveCollision(map);
    }

    void Zombie::solveCollision(Interfaces::IMap* map) {
        if (!map) return;
        
        glm::vec3 minExtents = m_position - glm::vec3(0.3f, 0.0f, 0.3f);
        glm::vec3 maxExtents = m_position + glm::vec3(0.3f, 1.8f, 0.3f);

        int minX = (int)std::floor(minExtents.x);
        int maxX = (int)std::ceil(maxExtents.x);
        int minY = (int)std::floor(minExtents.y);
        int maxY = (int)std::ceil(maxExtents.y);
        int minZ = (int)std::floor(minExtents.z);
        int maxZ = (int)std::ceil(maxExtents.z);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    if (map->isSolid(x, y, z)) {
                        glm::vec3 bMin(x, y, z);
                        glm::vec3 bMax(x + 1, y + 1, z + 1);

                        if ((minExtents.x < bMax.x && maxExtents.x > bMin.x) &&
                            (minExtents.y < bMax.y && maxExtents.y > bMin.y) &&
                            (minExtents.z < bMax.z && maxExtents.z > bMin.z)) {
                            
                            float dx1 = bMax.x - minExtents.x;
                            float dx2 = maxExtents.x - bMin.x;
                            float dy1 = bMax.y - minExtents.y;
                            float dy2 = maxExtents.y - bMin.y;
                            float dz1 = bMax.z - minExtents.z;
                            float dz2 = maxExtents.z - bMin.z;

                            float penX = std::min(dx1, dx2);
                            float penY = std::min(dy1, dy2);
                            float penZ = std::min(dz1, dz2);

                            if (penX < penY && penX < penZ) {
                                if (dx1 < dx2) m_position.x += penX; else m_position.x -= penX;
                                if (std::abs(m_velocity.y) < 0.1f) {
                                    m_velocity.y = 10.0f; // Jump
                                }
                                m_velocity.x = 0;
                            } else if (penY < penX && penY < penZ) {
                                if (dy1 < dy2) {
                                    m_position.y += penY;
                                    if (m_velocity.y < 0) m_velocity.y = 0;
                                } else {
                                    m_position.y -= penY;
                                    if (m_velocity.y > 0) m_velocity.y = 0;
                                }
                            } else {
                                if (dz1 < dz2) m_position.z += penZ; else m_position.z -= penZ;
                                if (std::abs(m_velocity.y) < 0.1f) {
                                    m_velocity.y = 10.0f; // Jump
                                }
                                m_velocity.z = 0;
                            }
                            
                            minExtents = m_position - glm::vec3(0.3f, 0.0f, 0.3f);
                            maxExtents = m_position + glm::vec3(0.3f, 1.8f, 0.3f);
                        }
                    }
                }
            }
        }
    }

    void Zombie::initMesh() {
        // A simple centered cube (0.6x0.6x1.8 approx, shifted to stand on origin)
        float verts[] = {
            // Back face (Z = -0.3)
            -0.3f, 0.0f, -0.3f,  0.0f, 0.0f,
             0.3f, 1.8f, -0.3f,  1.0f, 1.0f,
             0.3f, 0.0f, -0.3f,  1.0f, 0.0f,
             0.3f, 1.8f, -0.3f,  1.0f, 1.0f,
            -0.3f, 0.0f, -0.3f,  0.0f, 0.0f,
            -0.3f, 1.8f, -0.3f,  0.0f, 1.0f,
            // Front face (Z = 0.3)
            -0.3f, 0.0f,  0.3f,  0.0f, 0.0f,
             0.3f, 0.0f,  0.3f,  1.0f, 0.0f,
             0.3f, 1.8f,  0.3f,  1.0f, 1.0f,
             0.3f, 1.8f,  0.3f,  1.0f, 1.0f,
            -0.3f, 1.8f,  0.3f,  0.0f, 1.0f,
            -0.3f, 0.0f,  0.3f,  0.0f, 0.0f,
            // Left face (X = -0.3)
            -0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
            -0.3f, 1.8f, -0.3f,  1.0f, 1.0f,
            -0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
            -0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
            -0.3f, 0.0f,  0.3f,  0.0f, 0.0f,
            -0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
            // Right face (X = 0.3)
             0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
             0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
             0.3f, 1.8f, -0.3f,  1.0f, 1.0f,
             0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
             0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
             0.3f, 0.0f,  0.3f,  0.0f, 0.0f,
            // Bottom face (Y = 0.0)
            -0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
             0.3f, 0.0f, -0.3f,  1.0f, 1.0f,
             0.3f, 0.0f,  0.3f,  1.0f, 0.0f,
             0.3f, 0.0f,  0.3f,  1.0f, 0.0f,
            -0.3f, 0.0f,  0.3f,  0.0f, 0.0f,
            -0.3f, 0.0f, -0.3f,  0.0f, 1.0f,
            // Top face (Y = 1.8)
            -0.3f, 1.8f, -0.3f,  0.0f, 1.0f,
             0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
             0.3f, 1.8f, -0.3f,  1.0f, 1.0f,
             0.3f, 1.8f,  0.3f,  1.0f, 0.0f,
            -0.3f, 1.8f, -0.3f,  0.0f, 1.0f,
            -0.3f, 1.8f,  0.3f,  0.0f, 0.0f
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Zombie::render(Interfaces::IRenderer* renderer) {
        // Just bind VAO and draw. Assume Shader is bound remotely and model matrix is updated.
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

}
}
