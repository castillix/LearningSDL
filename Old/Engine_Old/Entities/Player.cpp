#include "Engine/Entities/Player.hpp"
#include "Engine/Game/VoxelMap.hpp"

namespace Entities {
    Player::Player(glm::vec3 spawnPos) 
        : m_position(spawnPos), m_velocity(0.0f), m_isGrounded(false), 
          m_health(100), m_maxHealth(100), m_points(500), m_activeWeaponIndex(0) {
        m_camera = Graphics::Camera3D(spawnPos + glm::vec3(0, 1.5f, 0)); // Eye height
        
        // Starting pistol
        m_weapons.push_back(Weapon("M1911", 20, 8, 40, 0.2f, 1.5f));
    }

    Physics::AABB Player::getAABB() const {
        // Player is roughly 0.6 units wide and 1.8 units tall
        return Physics::AABB(m_position - glm::vec3(0.3f, 0.0f, 0.3f), m_position + glm::vec3(0.3f, 1.8f, 0.3f));
    }

    void Player::takeDamage(int amount) {
        m_health -= amount;
        if (m_health < 0) m_health = 0;
    }

    bool Player::spendPoints(int amount) {
        if (m_points >= amount) {
            m_points -= amount;
            return true;
        }
        return false;
    }

    void Player::update(float deltaTime, Game::VoxelMap* map) {
        if (Weapon* w = getActiveWeapon()) {
            w->update(deltaTime);
        }

        // Simple input to acceleration
        glm::vec3 accel(0.0f);
        float speed = 20.0f;
        
        glm::vec3 forward = glm::normalize(glm::vec3(m_camera.getFront().x, 0, m_camera.getFront().z));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));

        if (moveForward) accel += forward;
        if (moveBackward) accel -= forward;
        if (moveLeft) accel -= right;
        if (moveRight) accel += right;
        
        if (glm::length(accel) > 0.0f) {
            accel = glm::normalize(accel) * speed;
        }

        // Apply friction
        m_velocity.x *= 0.8f; 
        m_velocity.z *= 0.8f; 

        m_velocity.x += accel.x * deltaTime;
        m_velocity.z += accel.z * deltaTime;

        // Apply Gravity
        m_velocity.y -= 30.0f * deltaTime;
        
        if (jump && m_isGrounded) {
            m_velocity.y = 10.0f;
            m_isGrounded = false;
        }

        m_position += m_velocity * deltaTime;
        solveCollision(map);

        // Update camera position
        m_camera.setPosition(m_position + glm::vec3(0, 1.5f, 0)); // Eye height
    }

    void Player::solveCollision(Game::VoxelMap* map) {
        if (!map) return;
        m_isGrounded = false;
        
        // Test blocks around the player
        int minX = (int)floor(m_position.x - 0.5f);
        int maxX = (int)ceil(m_position.x + 0.5f);
        int minY = (int)floor(m_position.y - 0.5f);
        int maxY = (int)ceil(m_position.y + 2.0f);
        int minZ = (int)floor(m_position.z - 0.5f);
        int maxZ = (int)ceil(m_position.z + 0.5f);

        Physics::AABB pBox = getAABB();

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    if (map->getBlock(x, y, z) != 0) {
                        Physics::AABB blockBox(glm::vec3(x, y, z), glm::vec3(x + 1, y + 1, z + 1));
                        if (pBox.intersects(blockBox)) {
                            // Resolve the collision on the axis with the least penetration
                            float dx1 = blockBox.maxExtents.x - pBox.minExtents.x;
                            float dx2 = pBox.maxExtents.x - blockBox.minExtents.x;
                            float dy1 = blockBox.maxExtents.y - pBox.minExtents.y;
                            float dy2 = pBox.maxExtents.y - blockBox.minExtents.y;
                            float dz1 = blockBox.maxExtents.z - pBox.minExtents.z;
                            float dz2 = pBox.maxExtents.z - blockBox.minExtents.z;

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
                            pBox = getAABB(); // Update bounding box for next test
                        }
                    }
                }
            }
        }
    }
}
