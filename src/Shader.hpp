#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public: 
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    GLunit id() const;

    void setUniform(const std::string& name, const glm::mat4& mat) const;
    void setUniform(const std::string& name, const glm::vec3& vec) const;
    void setUniform(const std::string& name, float value) const;

private:
    GLunit programID;
    std::string loadFile(const std::string& path) const;
    GLunit compile(GLenum type, const std::string source) const;    
};