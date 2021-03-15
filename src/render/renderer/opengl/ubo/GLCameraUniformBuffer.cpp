#include "render/renderer/opengl/ubo/GLCameraUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) CameraData
{
    CameraData(const math::Matrix4x4& projection_matrix,
               const math::Matrix4x4& view_matrix,
               const math::Vec3f& camera_position)
    : projection_matrix_(projection_matrix)
    , view_matrix_(view_matrix)
    , camera_position_(camera_position.x_,
                       camera_position.y_,
                       camera_position.z_,
                       1.0F)
    {
    }

    math::Matrix4x4 projection_matrix_;
    math::Matrix4x4 view_matrix_;
    math::Vec4f camera_position_;
};

GLCameraUniformBuffer::GLCameraUniformBuffer()
: GLBaseUniformBuffer("Camera")
{
}

void GLCameraUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<CameraData>(binding_index);
}

void GLCameraUniformBuffer::UpdateUniforms(const math::Matrix4x4& projection_matrix,
                                           const math::Matrix4x4& view_matrix,
                                           const math::Vec3f& camera_position)
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_projection_matrix = projection_matrix.Transpose();
    math::Matrix4x4 transpose_view_matrix = view_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    CameraData camera_data{transpose_projection_matrix, transpose_view_matrix, camera_position};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &camera_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render