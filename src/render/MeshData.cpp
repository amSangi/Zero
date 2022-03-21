#include "render/MeshData.hpp"

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

MeshData::MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
: MeshData(std::move(vertices), std::move(indices), std::vector<std::string>())
{
}

MeshData::MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices, std::vector<std::string>&& bone_names)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
, bone_names_(std::move(bone_names))
{
}

MeshData::MeshData(MeshData&& mesh_data) noexcept
: vertices_(std::move(mesh_data.vertices_))
, indices_(std::move(mesh_data.indices_))
, bone_names_(std::move(mesh_data.bone_names_))
{
}

} // namespace zero::render