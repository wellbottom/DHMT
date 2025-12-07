#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    GLuint ID;

    // Constructor reads and builds the shader
    Shader(const char* vertexSource, const char* fragmentSource);

    // Use/activate the shader
    void use();

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // Cleanup
    void deleteProgram();

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif