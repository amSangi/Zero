#include "component/SkyDome.hpp"

namespace zero
{

SkyDome::SkyDome()
: Component()
, is_active_(false)
, apex_color_(math::Vec3f(0.0F, 0.15F, 0.66F))
, center_color_(math::Vec3f(0.3F, 0.45F, 0.96F))
, shader_id_(0)
, vertex_shader_()
, fragment_shader_()
{
	ComputeShaderID();
}

void SkyDome::SetShaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
	vertex_shader_ = vertex_shader;
	fragment_shader_ = fragment_shader;
	ComputeShaderID();
}

const std::string& SkyDome::GetVertexShader() const
{
	return vertex_shader_;
}

const std::string& SkyDome::GetFragmentShader() const
{
	return fragment_shader_;
}

uint32 SkyDome::GetShaderID() const
{
	return shader_id_;
}

void SkyDome::ComputeShaderID()
{
	std::string shader_aggregate{};
	shader_aggregate += vertex_shader_;
	shader_aggregate += fragment_shader_;
	shader_id_ = std::hash<std::string>()(shader_aggregate);
}

} // namespace zero

