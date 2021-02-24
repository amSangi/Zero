#include "render/IMesh.hpp"

namespace zero::render
{

Vertex::Vertex()
: position_(math::Vec3f::Zero())
, normal_(math::Vec3f::Zero())
, texture_coordinate_(math::Vec2f::Zero())
, bone_ids_(math::Vec4i(kInvalidBoneID))
, bone_weights_(math::Vec4f::Zero())
{
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
{
}

Mesh::Mesh(Mesh&& mesh) noexcept
: vertices_(std::move(mesh.vertices_))
, indices_(std::move(mesh.indices_))
{
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return vertices_;
}

const std::vector<zero::uint32>& Mesh::GetIndices() const
{
    return indices_;
}

} // namespace zero::render