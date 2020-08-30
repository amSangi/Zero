#include "component/Material.hpp"

namespace zero
{

Material::Material()
: name_()
, texture_map_()
, shaders_()
, diffuse_color_(math::Vec3f::Zero())
, specular_intensity_(1.0F)
, specular_exponent_(32.0F)
, two_sided_(false)
, wireframe_enabled_(false)
, visible_(true)
{
}

} // namespace zero