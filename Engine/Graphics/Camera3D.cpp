#include "Engine/Graphics/Camera3D.hpp"
#include <algorithm>

namespace Engine {
namespace Graphics {

    Camera3D::Camera3D(glm::vec3 position)
        : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
          m_yaw(-90.0f), m_pitch(0.0f), m_mouseSensitivity(0.1f) {
        updateCameraVectors();
    }

    glm::mat4 Camera3D::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera3D::processMouse(float xoffset, float yoffset, bool constrainPitch) {
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
}
