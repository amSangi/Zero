#include "render/renderer/opengl/GLMesh.hpp"

namespace zero::render
{

GLMesh::GLMesh(GLuint vao_identifier, GLuint vertex_buffer, GLuint index_buffer, uint32 index_data_size)
: vao_id_(vao_identifier)
, vertex_buffer_id_(vertex_buffer)
, index_buffer_id_(index_buffer)
, index_data_size_(index_data_size)
{
}

GLuint GLMesh::GetVAOIdentifier() const
{
    return vao_id_;
}

GLuint GLMesh::GetVertexBufferIdentifier() const
{
    return vertex_buffer_id_;
}

GLuint GLMesh::GetIndexBufferIdentifier() const
{
    return index_buffer_id_;
}

uint32 GLMesh::GetIndexDataSize() const
{
    return index_data_size_;
}

} // namespace zero::render