#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Engine {
namespace Graphics {

    class Shader {
    public:
        Shader() = default;
        ~Shader();

        bool compile(const std::string& vertexPath, const std::string& fragmentPath);
        void use();

        void setMat4(const std::string& name, const glm::mat4& mat) const;
        void setVec3(const std::string& name, const glm::vec3& vec) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;

    private:
        unsigned int m_id = 0;
        void checkCompileErrors(unsigned int shader, std::string type);
    };

}
}
