#include "render/Material.hpp"

namespace zero::render
{

Material::Material()
: name_()
, texture_map_()
, shaders_()
, diffuse_color_(math::Vec3f::Zero())
, specular_exponent_(32.0F)
, wireframe_enabled_(false)
, visible_(true)
{
}

} // namespace zero::render