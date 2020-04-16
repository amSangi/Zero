#include "render/Light.hpp"

using namespace zero::render;

Light::LightSource::LightSource(const DirectionalLight& directional_light)
: directional_light_(directional_light)
{}

Light::LightSource::LightSource(const PointLight& point_light)
: point_light_(point_light)
{}

Light::LightSource::LightSource(const SpotLight& spot_light)
: spot_light_(spot_light)
{}


Light::Light()
: type_(Light::Type::POINT)
, light_source_(PointLight())
{}

Light::Light(const DirectionalLight& directional_light)
: type_(Light::Type::DIRECTIONAL)
, light_source_(directional_light)
{}

Light::Light(const PointLight& point_light)
: type_(Light::Type::POINT)
, light_source_(point_light)
{}

Light::Light(const SpotLight& spot_light)
: type_(Light::Type::SPOT)
, light_source_(spot_light)
{}

Light::Type Light::GetType() const {
    return type_;
}

DirectionalLight Light::GetDirectionalLight() const {
    if (type_ != Type::DIRECTIONAL) return DirectionalLight();
    return light_source_.directional_light_;
}

PointLight Light::GetPointLight() const {
    if (type_ != Type::POINT) return PointLight();
    return light_source_.point_light_;
}

SpotLight Light::GetSpotLight() const {
    if (type_ != Type::SPOT) return SpotLight();
    return light_source_.spot_light_;
}

void Light::Set(const DirectionalLight& directional_light) {
    type_ = Type::DIRECTIONAL;
    light_source_.directional_light_ = directional_light;
}

void Light::Set(const PointLight& point_light) {
    type_ = Type::POINT;
    light_source_.point_light_ = point_light;
}

void Light::Set(const SpotLight& spot_light) {
    type_ = Type::SPOT;
    light_source_.spot_light_ = spot_light;
}

