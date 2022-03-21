#include "component/Material.hpp"

namespace zero
{

Material::Material()
: Component()
, name_()
, diffuse_color_(math::Vec3f::Zero())
, specular_intensity_(1.0F)
, specular_exponent_(32.0F)
, two_sided_(false)
, wireframe_enabled_(false)
, visible_(true)
, texture_map_()
, shaders_()
, texture_id_(0)
, shader_id_(0)
{
    ComputeTextureID();
    ComputeShaderID();
}

const Material::TextureMap& Material::GetTextureMap() const
{
    return texture_map_;
}

const Material::Shaders& Material::GetShaders() const
{
    return shaders_;
}

uint32 Material::GetTextureID() const
{
    return texture_id_;
}

uint32 Material::GetShaderID() const
{
    return shader_id_;
}

void Material::SetTextureMap(const TextureMap& texture_map)
{
    texture_map_ = texture_map;
    ComputeTextureID();
}

void Material::SetShaders(const Shaders& shaders)
{
    shaders_ = shaders;
    ComputeShaderID();
}

void Material::ComputeTextureID()
{
    std::string texture_aggregate{};
    texture_aggregate += texture_map_.diffuse_map_;
    texture_aggregate += texture_map_.normal_map_;
    texture_aggregate += texture_map_.specular_map_;
    texture_id_ = std::hash<std::string>()(texture_aggregate);
}

void Material::ComputeShaderID()
{
    std::string shader_aggregate{};
    shader_aggregate += shaders_.vertex_shader_;
    shader_aggregate += shaders_.fragment_shader_;
    shader_aggregate += shaders_.geometry_shader_;
    shader_aggregate += shaders_.tessellation_ctrl_shader_;
    shader_aggregate += shaders_.tessellation_eval_shader_;
    shader_aggregate += shaders_.compute_shader_;
    shader_id_ = std::hash<std::string>()(shader_aggregate);
}

} // namespace zero