#include "render/opengl/GLMesh.hpp"

namespace zero::render
{

GLMesh::GLMesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: Mesh(std::move(vertices), std::move(indices))
{
}

GLMesh::GLMesh(Mesh&& mesh)
: Mesh(std::move(mesh))
{
}

void GLMesh::Draw()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &vertices_[0].position_);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), &vertices_[0].normal_);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), &vertices_[0].texture_coordinate_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, indices_.data());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

} // namespace zero::render