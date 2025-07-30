#include "Mesh.hpp"

Mesh::Mesh(const std::vector<float>& vertices, GLenum drawMode) : mode(drawMode), vertexCount(vertices.size()/3) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}