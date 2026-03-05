#pragma once
#include <glm/glm.hpp>
#include "Engine/Physics/AABB.hpp"
#include "Engine/Graphics/Camera3D.hpp"
#include "Engine/Entities/Weapon.hpp"
#include <vector>

namespace Game { class VoxelMap; }

namespace Entities {
    class Player {
    public:
        Player(glm::vec3 spawnPos);

        void update(float deltaTime, Game::VoxelMap* map);

        glm::vec3 getPosition() const { return m_position; }
        Graphics::Camera3D& getCamera() { return m_camera; }
        Physics::AABB getAABB() const;

        void takeDamage(int amount);
        void addPoints(int amount) { m_points += amount; }
        bool spendPoints(int amount);
        bool isDead() const { return m_health <= 0; }
        
        int getHealth() const { return m_health; }
        int getPoints() const { return m_points; }
        
        std::vector<Weapon>& getWeapons() { return m_weapons; }
        int getActiveWeaponIndex() const { return m_activeWeaponIndex; }

        void giveWeapon(const Weapon& weapon) {
            m_weapons.push_back(weapon);
            m_activeWeaponIndex = m_weapons.size() - 1;
        }

        Weapon* getActiveWeapon() { 
            if (m_weapons.empty() || m_activeWeaponIndex >= m_weapons.size()) return nullptr;
            return &m_weapons[m_activeWeaponIndex]; 
        }

        // Input flags
        bool moveForward = false;
        bool moveBackward = false;
        bool moveLeft = false;
        bool moveRight = false;
        bool jump = false;
        
        // Mouse input directly feeds the camera
        void processMouse(float xoffset, float yoffset) {
            m_camera.processMouseMovement(xoffset, yoffset);
        }

    private:
        glm::vec3 m_position;
        glm::vec3 m_velocity;
        Graphics::Camera3D m_camera;

        int m_health;
        int m_maxHealth;
        int m_points;

        std::vector<Weapon> m_weapons;
        int m_activeWeaponIndex;

        bool m_isGrounded;

        void solveCollision(Game::VoxelMap* map);
    };
}
