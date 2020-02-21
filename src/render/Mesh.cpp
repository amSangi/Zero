#include "render/Mesh.hpp"

using namespace zero::render;

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
{}

const std::vector<Vertex>& Mesh::GetVertices() const {
    return vertices_;
}

const std::vector<zero::uint32>& Mesh::GetIndices() const {
    return indices_;
}