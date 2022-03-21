#include "component/SkyDome.hpp"

namespace zero
{

SkyDome::SkyDome()
: Component()
, is_active_(false)
, vertex_shader_()
, fragment_shader_()
, apex_color_(math::Vec3f(0.0F, 0.15F, 0.66F))
, center_color_(math::Vec3f(0.3F, 0.45F, 0.96F))
{
}

} // namespace zero

