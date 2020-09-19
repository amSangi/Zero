#include "render/opengl/GLMesh.hpp"

namespace zero::render
{

GLMesh::GLMesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: Mesh(std::move(vertices), std::move(indices))
, vao_(0)
, vertex_vbo_(0)
, index_vbo_(0)
{
}

GLMesh::GLMesh(Mesh&& mesh)
: Mesh(std::move(mesh))
, vao_(0)
, vertex_vbo_(0)
, index_vbo_(0)
{
}

GLMesh::~GLMesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertex_vbo_);
    glDeleteBuffers(1, &index_vbo_);
}

void GLMesh::Initialize()
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    constexpr uint32 position_attribute_index = 0;
    constexpr uint32 normal_attribute_index = 1;
    constexpr uint32 uv_attribute_index = 2;
    glEnableVertexAttribArray(position_attribute_index);
    glEnableVertexAttribArray(normal_attribute_index);
    glEnableVertexAttribArray(uv_attribute_index);
    glVertexAttribPointer(position_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position_)));
    glVertexAttribPointer(normal_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal_)));
    glVertexAttribPointer(uv_attribute_index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture_coordinate_)));

    glGenBuffers(1, &index_vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLMesh::Draw()
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace zero::render