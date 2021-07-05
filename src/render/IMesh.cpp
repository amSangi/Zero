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
: Mesh(std::move(vertices), std::move(indices), std::vector<std::string>())
{
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices, std::vector<std::string>&& bone_names)
: vertices_(std::move(vertices))
, indices_(std::move(indices))
, bone_names_(std::move(bone_names))
{
}

Mesh::Mesh(Mesh&& mesh)
: vertices_(std::move(mesh.vertices_))
, indices_(std::move(mesh.indices_))
, bone_names_(std::move(mesh.bone_names_))
{
}

} // namespace zero::render