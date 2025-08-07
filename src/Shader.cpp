#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertCode = loadFile(vertexPath);
    std::string fragCode = loadFile(fragmentPath);

    GLuint vert = compile(GL_VERTEX_SHADER, vertCode);
    GLuint frag = compile(GL_FRAGMENT_SHADER, fragCode);

    programID = glCreateProgram();
    glAttachShader(programID, vert);
    glAttachShader(programID, frag);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << "\n";
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const { glUseProgram(programID); }
GLuint Shader::id() const { return programID; }

std::string Shader::loadFile(const std::string& path) const {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string& source) const {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << (type==GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compile error: " << infoLog << "\n";
    }
    return shader;
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(const std::string& name, const glm::vec3& vec) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(loc, 1, &vec[0]);
}
void Shader::setUniform(const std::string& name, float value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1f(loc, value);
}