#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
namespace Graphics {

    class Camera3D {
    public:
        Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

        glm::mat4 getViewMatrix() const;
        
        glm::vec3 getPosition() const { return m_position; }
        void setPosition(const glm::vec3& pos) { m_position = pos; }

        glm::vec3 getFront() const { return m_front; }
        glm::vec3 getRight() const { return m_right; }
        glm::vec3 getUp() const { return m_up; }

        void processMouse(float xoffset, float yoffset, bool constrainPitch = true);
        float getYaw() const { return m_yaw; }
        float getPitch() const { return m_pitch; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float m_yaw;
        float m_pitch;
        float m_mouseSensitivity;

        void updateCameraVectors();
    };

}
}
