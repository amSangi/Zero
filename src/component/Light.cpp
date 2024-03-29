#include "component/Light.hpp"

namespace zero
{

Attenuation::Attenuation()
: constant_(1.0F)
, linear_(0.09F)
, quadratic_(0.032F)
{
}

DirectionalLight::DirectionalLight()
: Component()
, color_(math::Vec3f::One())
, direction_(math::Vec3f::Down())
, ambient_intensity_(1.0F)
, diffuse_intensity_(1.0F)
, casts_shadows_(false)
{
}

PointLight::PointLight()
: Component()
, color_(math::Vec3f::One())
, ambient_intensity_(1.0F)
, diffuse_intensity_(1.0F)
, attenuation_()
, casts_shadows_(false)
{
}

SpotLight::SpotLight()
: Component()
, color_(math::Vec3f::One())
, direction_(math::Vec3f::Down())
, ambient_intensity_(1.0F)
, diffuse_intensity_(1.0F)
, inner_cone_angle_(math::Degree(12.5F))
, outer_cone_angle_(math::Degree(17.5F))
, attenuation_()
, casts_shadows_(false)
{
}

Light::LightSource::LightSource(const DirectionalLight& directional_light)
: directional_light_(directional_light)
{
}

Light::LightSource::LightSource(const PointLight& point_light)
: point_light_(point_light)
{
}

Light::LightSource::LightSource(const SpotLight& spot_light)
: spot_light_(spot_light)
{
}


Light::Light()
: type_(Light::Type::POINT)
, source_(PointLight())
{
}

Light::Light(const DirectionalLight& directional_light)
: type_(Light::Type::DIRECTIONAL)
, source_(directional_light)
{
}

Light::Light(const PointLight& point_light)
: type_(Light::Type::POINT)
, source_(point_light)
{
}

Light::Light(const SpotLight& spot_light)
: type_(Light::Type::SPOT)
, source_(spot_light)
{
}

Light::Type Light::GetType() const
{
    return type_;
}

DirectionalLight Light::GetDirectionalLight() const
{
    if (type_ != Type::DIRECTIONAL)
    {
        return DirectionalLight();
    }
    return source_.directional_light_;
}

PointLight Light::GetPointLight() const
{
    if (type_ != Type::POINT)
    {
        return PointLight();
    }
    return source_.point_light_;
}

SpotLight Light::GetSpotLight() const
{
    if (type_ != Type::SPOT)
    {
        return SpotLight();
    }
    return source_.spot_light_;
}

void Light::Set(const DirectionalLight& directional_light)
{
    type_ = Type::DIRECTIONAL;
    source_.directional_light_ = directional_light;
}

void Light::Set(const PointLight& point_light)
{
    type_ = Type::POINT;
    source_.point_light_ = point_light;
}

void Light::Set(const SpotLight& spot_light)
{
    type_ = Type::SPOT;
    source_.spot_light_ = spot_light;
}

} // namespace zero