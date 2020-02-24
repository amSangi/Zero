#include "render/Mesh.hpp"

using namespace zero::render;

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
{}

Mesh::Mesh(Mesh&& mesh) noexcept
: vertices_(std::move(mesh.vertices_))
, indices_(std::move(mesh.indices_))
{}

const std::vector<Vertex>& Mesh::GetVertices() const {
    return vertices_;
}

const std::vector<zero::uint32>& Mesh::GetIndices() const {
    return indices_;
}