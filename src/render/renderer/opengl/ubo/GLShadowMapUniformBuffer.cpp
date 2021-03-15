#include "render/renderer/opengl/ubo/GLShadowMapUniformBuffer.hpp"
#include "render/Constants.hpp"

namespace zero::render
{

struct alignas(16) ShadowMapInformation
{
    ShadowMapInformation(const std::vector<math::Matrix4x4>& shadow_map_matrices,
                         const std::vector<float>& cascade_far_bounds)
    : cascade_far_bounds_()
    , shadow_map_matrices_()
    {
        for (uint32 i = 0; i < Constants::kShadowCascadeCount; ++i)
        {
            shadow_map_matrices_[i] = shadow_map_matrices[i].Transpose();
            cascade_far_bounds_[i].z_ = cascade_far_bounds[i];
        }
    }

    math::Matrix4x4 shadow_map_matrices_[Constants::kShadowCascadeCount];
    math::Vec4f cascade_far_bounds_[Constants::kShadowCascadeCount];
};

GLShadowMapUniformBuffer::GLShadowMapUniformBuffer()
: GLBaseUniformBuffer("ShadowMapInformation")
{
}

void GLShadowMapUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<ShadowMapInformation>(binding_index);
}

void GLShadowMapUniformBuffer::UpdateUniforms(const std::vector<math::Matrix4x4>& shadow_map_matrices,
                                              const std::vector<float>& cascade_end_clip_spaces)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    ShadowMapInformation shadow_map_information{shadow_map_matrices, cascade_end_clip_spaces};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShadowMapInformation), &shadow_map_information);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render