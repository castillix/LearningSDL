#include "Engine/Graphics/Camera3D.hpp"

namespace Graphics {
    Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
        : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(5.0f), m_mouseSensitivity(0.1f) {
        
        m_position = position;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 Camera3D::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera3D::processKeyboard(int direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        if (direction == 1) // FORWARD
            m_position += m_front * velocity;
        if (direction == 2) // BACKWARD
            m_position -= m_front * velocity;
        if (direction == 3) // LEFT
            m_position -= m_right * velocity;
        if (direction == 4) // RIGHT
            m_position += m_right * velocity;
    }

    void Camera3D::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch) {
            if (m_pitch > 89.0f) m_pitch = 89.0f;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera3D::updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up    = glm::normalize(glm::cross(m_right, m_front));
    }
}
