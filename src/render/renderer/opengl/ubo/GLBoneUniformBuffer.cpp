#include "render/renderer/opengl/ubo/GLBoneUniformBuffer.hpp"
#include "render/Constants.hpp"
#include <assert.h>

namespace zero::render
{

GLBoneUniformBuffer::GLBoneUniformBuffer()
: GLBaseUniformBuffer("Bones")
{
}

void GLBoneUniformBuffer::Initialize(uint32 binding_index)
{
    // Last matrix will always be the identity matrix.
    // The identity matrix is applied to a vertex if the bone ID is invalid
    InitializeBaseBuffer(binding_index, sizeof(math::Matrix4x4) * (Constants::kMaxMeshBoneCount + 1));

    // Set identity matrices
    std::vector<math::Matrix4x4> bone_matrices{Constants::kMaxMeshBoneCount + 1, math::Matrix4x4::Identity()};
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(math::Matrix4x4) * bone_matrices.size(), bone_matrices.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLBoneUniformBuffer::UpdateUniforms(std::vector<math::Matrix4x4> bone_matrices)
{
    assert(bone_matrices.size() <= Constants::kMaxMeshBoneCount);

    // Transpose each bone matrix
    for (math::Matrix4x4& matrix : bone_matrices)
    {
        matrix = matrix.Transpose();
    }
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(math::Matrix4x4) * bone_matrices.size(), bone_matrices.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render

