#include "render/opengl/GLMesh.hpp"

namespace zero::render
{

GLMesh::GLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices)
: vao_(0)
, vertex_vbo_(0)
, index_vbo_(0)
, index_data_size_(0)
{
    index_data_size_ = indices.size();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(const Mesh& mesh)
: GLMesh(mesh.GetVertices(), mesh.GetIndices())
{
}

GLMesh::~GLMesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertex_vbo_);
    glDeleteBuffers(1, &index_vbo_);
}

void GLMesh::Draw()
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, index_data_size_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace zero::render