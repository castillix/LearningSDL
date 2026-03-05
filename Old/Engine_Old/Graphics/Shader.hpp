#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace Graphics {
    class Shader {
    public:
        GLuint m_id;
        
        Shader();
        ~Shader();

        bool compile(const std::string& vertexPath, const std::string& fragmentPath);
        void use();

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        bool checkCompileErrors(GLuint shader, std::string type);
    };
}
