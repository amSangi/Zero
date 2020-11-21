#include "render/opengl/pipeline/GLSkyDomePass.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLMesh.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLUniformManager.hpp"

namespace zero::render
{

GLSkyDomePass::GLSkyDomePass(GLCompiler* gl_compiler,
                             GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                             GLUniformManager* gl_uniform_manager)
: gl_compiler_(gl_compiler)
, gl_primitive_mesh_manager_(gl_primitive_mesh_manager)
, gl_uniform_manager_(gl_uniform_manager)
, gl_sphere_(nullptr)
{
    PrimitiveInstance primitive_instance{};
    primitive_instance.Set(Sphere());
    gl_sphere_ = gl_primitive_mesh_manager_->GetPrimitiveMesh(primitive_instance);
}

void GLSkyDomePass::Execute(const Camera& camera, const entt::registry& registry)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    UpdateGLSettings(camera);

    math::Matrix4x4 projection_matrix = camera.GetProjectionMatrix();
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();
    gl_uniform_manager_->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);

    // Render the first active sky dome
    auto sky_dome_view = registry.view<const SkyDome>();
    for (Entity sky_dome_entity : sky_dome_view)
    {
        const SkyDome& sky_dome = sky_dome_view.get<const SkyDome>(sky_dome_entity);
        if (sky_dome.is_active_)
        {
            RenderSkyDome(camera, sky_dome);
            break;
        }
    }
}

void GLSkyDomePass::UpdateGLSettings(const Camera& camera) const
{
    glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
}

void GLSkyDomePass::RenderSkyDome(const Camera& camera, const SkyDome& sky_dome)
{
    constexpr float sky_dome_sphere_scale = 1000.0F;

    // Position sky dome sphere around the camera position
    math::Matrix4x4 model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(sky_dome_sphere_scale))
            .Translate(camera.position_);

    // Construct sky dome graphics program
    std::shared_ptr<IProgram> sky_dome_program = gl_compiler_->CreateProgram(sky_dome);
    sky_dome_program->Use();
    sky_dome_program->SetUniform("u_projection_matrix", camera.GetProjectionMatrix());
    sky_dome_program->SetUniform("u_view_matrix", camera.GetViewMatrix());
    sky_dome_program->SetUniform("u_model_matrix", model_matrix);
    sky_dome_program->SetUniform("u_camera_position", camera.position_);
    sky_dome_program->SetUniform("u_apex_color", sky_dome.apex_color_);
    sky_dome_program->SetUniform("u_center_color", sky_dome.center_color_);
    sky_dome_program->FlushUniforms();

    // Render sky dome
    gl_sphere_->Draw();
    sky_dome_program->Finish();
}

} // namespace zero::render

