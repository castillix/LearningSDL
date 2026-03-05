#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Graphics {
    class Camera3D {
    public:
        Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

        glm::mat4 getViewMatrix() const;
        void processKeyboard(int direction, float deltaTime); // 1=F, 2=B, 3=L, 4=R
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        glm::vec3 getPosition() const { return m_position; }
        void setPosition(const glm::vec3& pos) { m_position = pos; }
        glm::vec3 getFront() const { return m_front; }
        
        float m_movementSpeed;
        float m_mouseSensitivity;

    private:
        void updateCameraVectors();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float m_yaw;
        float m_pitch;
    };
}
