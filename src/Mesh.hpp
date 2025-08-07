#pragma once
#include <vector>
#include <glad/glad.h>

class Mesh {
    public:
        Mesh(const std::vector<float>& vertices, GLenum drawMode = GL_TRIANGLES);
        ~Mesh();
        void draw() const;
    private:
        GLuint VAO, VBO;
        GLsizei vertexCount;
        GLenum mode;
};
