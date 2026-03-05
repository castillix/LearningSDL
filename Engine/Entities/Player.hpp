#pragma once
#include "Engine/Interfaces/IEntity.hpp"
#include "Engine/Graphics/Camera3D.hpp"
#include "Engine/Interfaces/IInput.hpp"

namespace Engine {
namespace Entities {

    class Player : public Interfaces::IEntity {
    public:
        Player(glm::vec3 spawnPos);
        ~Player() override = default;

        void update(float deltaTime, class Interfaces::IMap* map) override;
        void render(class Interfaces::IRenderer* renderer) override;

        void handleInput(const class Interfaces::IInput* input);
        
        Graphics::Camera3D& getCamera() { return m_camera; }
        glm::vec3 getPosition() const { return m_position; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_velocity;
        bool m_isGrounded;

        Graphics::Camera3D m_camera;

        bool moveForward = false;
        bool moveBackward = false;
        bool moveLeft = false;
        bool moveRight = false;
        bool jump = false;

        void solveCollision(Interfaces::IMap* map);
    };

}
}
