#include "render/renderer/opengl/GLMesh.hpp"

namespace zero::render
{

GLMesh::GLMesh()
: vao_(0)
, vertex_vbo_(0)
, index_vbo_(0)
, index_data_size_(0)
{
}

void GLMesh::Initialize(std::unique_ptr<Mesh> mesh)
{
    index_data_size_ = mesh->indices_.size();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertices_.size(), mesh->vertices_.data(), GL_STATIC_DRAW);

    constexpr uint32 position_attribute_index = 0;
    constexpr uint32 normal_attribute_index = 1;
    constexpr uint32 uv_attribute_index = 2;
    constexpr uint32 bone_id_attribute_index = 3;
    constexpr uint32 bone_weight_attribute_index = 4;
    glEnableVertexAttribArray(position_attribute_index);
    glEnableVertexAttribArray(normal_attribute_index);
    glEnableVertexAttribArray(uv_attribute_index);
    glEnableVertexAttribArray(bone_id_attribute_index);
    glEnableVertexAttribArray(bone_weight_attribute_index);
    glVertexAttribPointer(position_attribute_index,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position_)));
    glVertexAttribPointer(normal_attribute_index,      3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal_)));
    glVertexAttribPointer(uv_attribute_index,          2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture_coordinate_)));
    glVertexAttribPointer(bone_id_attribute_index,     4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bone_ids_)));
    glVertexAttribPointer(bone_weight_attribute_index, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bone_weights_)));

    glGenBuffers(1, &index_vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * mesh->indices_.size(), mesh->indices_.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLMesh::~GLMesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertex_vbo_);
    glDeleteBuffers(1, &index_vbo_);
}

} // namespace zero::render