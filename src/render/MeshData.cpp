#include "render/MeshData.hpp"

namespace zero::render
{

Vertex::Vertex()
: position_(math::Vec3f::Zero())
, normal_(math::Vec3f::Zero())
, texture_coordinate_(math::Vec2f::Zero())

{
}

MeshData::MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
{
}

MeshData::MeshData(MeshData&& mesh_data) noexcept
: vertices_(std::move(mesh_data.vertices_))
, indices_(std::move(mesh_data.indices_))
{
}

} // namespace zero::render