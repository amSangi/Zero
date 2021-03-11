#include "render/renderer/opengl/ubo/GLSpotLightUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) SpotLightData
{
    SpotLightData(const Transform& transform, const SpotLight& light)
    : position_(transform.GetPosition().x_,
                transform.GetPosition().y_,
                transform.GetPosition().z_,
                0.0F)
    , color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , inner_cosine_(math::Cos(light.inner_cone_angle_.ToRadian().rad_))
    , outer_cosine_(math::Cos(light.outer_cone_angle_.ToRadian().rad_))
    , attenuation_constant_(light.attenuation_.constant_)
    , attenuation_linear_(light.attenuation_.linear_)
    , attenuation_quadratic_(light.attenuation_.quadratic_)
    , padding_()
    {
    }

    math::Vec4f position_;
    math::Vec4f color_;
    math::Vec4f direction_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float inner_cosine_;
    float outer_cosine_;
    float attenuation_constant_;
    float attenuation_linear_;
    float attenuation_quadratic_;
    float padding_[1];
};

GLSpotLightUniformBuffer::GLSpotLightUniformBuffer()
: GLBaseUniformBuffer()
{
}

void GLSpotLightUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<SpotLightData>(binding_index);
}

void GLSpotLightUniformBuffer::UpdateUniforms(const std::vector<SpotLight>& spot_lights)
{
    std::vector<SpotLightData> spot_light_data{};
    spot_light_data.reserve(spot_lights.size());

    for (const SpotLight& spot_light : spot_lights)
    {
        spot_light_data.emplace_back(spot_light);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SpotLightData) * spot_light_data.size(), spot_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render