#include "render/opengl/GLMesh.hpp"

using namespace zero::render;

GLMesh::GLMesh(std::vector<Vertex>&& vertices, std::vector<uint16>&& indices)
: Mesh(std::move(vertices), std::move(indices))
, vao_(0)
, vbo_(0)
, ebo_(0)
{}

GLMesh::~GLMesh() {
    Cleanup();
}

void GLMesh::Cleanup() {
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
    ebo_ = 0;
    vbo_ = 0;
    vao_ = 0;
}

void GLMesh::Initialize() {
    Cleanup();

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    // Set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_.front(), GL_STATIC_DRAW);

    // Set index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32), &indices_.front(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position_));
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));
    // Texture Coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate_));

    glBindVertexArray(0);
}

void GLMesh::Draw() {
    if (!vao_) return;
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, indices_.size());
    glBindVertexArray(0);
}


