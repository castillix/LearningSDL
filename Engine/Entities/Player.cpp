#include "Engine/Entities/Player.hpp"
#include "Engine/Interfaces/IMap.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

namespace Engine {
namespace Entities {

    Player::Player(glm::vec3 spawnPos)
        : m_position(spawnPos), m_velocity(0.0f), m_isGrounded(false) {
        m_camera = Graphics::Camera3D(spawnPos + glm::vec3(0, 1.5f, 0));
    }

    void Player::handleInput(const Interfaces::IInput* input) {
        moveForward = input->isKeyHeld(SDL_SCANCODE_W);
        moveBackward = input->isKeyHeld(SDL_SCANCODE_S);
        moveLeft = input->isKeyHeld(SDL_SCANCODE_A);
        moveRight = input->isKeyHeld(SDL_SCANCODE_D);
        jump = input->isKeyHeld(SDL_SCANCODE_SPACE);

        float dx, dy;
        input->getMouseDelta(dx, dy);
        m_camera.processMouse(dx, -dy);
    }

    void Player::update(float deltaTime, Interfaces::IMap* map) {
        glm::vec3 accel(0.0f);
        float speed = 200.0f; // 10x Base Speed
        
        glm::vec3 forward = glm::normalize(glm::vec3(m_camera.getFront().x, 0, m_camera.getFront().z));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));

        if (moveForward) accel += forward;
        if (moveBackward) accel -= forward;
        if (moveLeft) accel -= right;
        if (moveRight) accel += right;
        
        if (glm::length(accel) > 0.0f) {
            accel = glm::normalize(accel) * speed;
        }

        // Apply friction (Increased slip for higher top speed control)
        m_velocity.x *= 0.5f; 
        m_velocity.z *= 0.5f; 

        m_velocity.x += accel.x * deltaTime;
        m_velocity.z += accel.z * deltaTime;

        // Gravity
        m_velocity.y -= 30.0f * deltaTime;
        
        if (jump && m_isGrounded) {
            m_velocity.y = 10.0f;
            m_isGrounded = false;
        }

        m_position += m_velocity * deltaTime;
        solveCollision(map);

        m_camera.setPosition(m_position + glm::vec3(0, 1.5f, 0));
    }

    void Player::solveCollision(Interfaces::IMap* map) {
        if (!map) return;
        m_isGrounded = false;
        
        // Simple AABB vs Voxel Map collision (with exclusive bounds)
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

                        // Intersection test
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
                            
                            // Re-eval Extents!
                            minExtents = m_position - glm::vec3(0.3f, 0.0f, 0.3f);
                            maxExtents = m_position + glm::vec3(0.3f, 1.8f, 0.3f);
                        }
                    }
                }
            }
        }
    }

    void Player::render(Interfaces::IRenderer* renderer) {
        // First-person player doesn't draw itself usually
    }

}
}
