#pragma once

#include "render/IRenderPass.hpp"
#include "component/Animator.hpp"
#include "component/Material.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "math/Matrix4x4.hpp"
#include <memory>

namespace zero::render
{
    // Forward declarations
    class GLCompiler;
    class GLMesh;
    class GLModelManager;
    class GLPrimitiveMeshManager;
    class GLProgram;
    class GLSampler;
    class GLTextureManager;
    class GLUniformManager;
    class IProgram;
    class Model;

    class GLEntityRenderPass : public IRenderPass
    {
    public:
        GLEntityRenderPass(GLCompiler* gl_compiler,
                           GLModelManager* gl_model_manager,
                           GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                           GLTextureManager* gl_texture_manager,
                           GLUniformManager* gl_uniform_manager);
        ~GLEntityRenderPass() override = default;

        /**
         * @brief IRenderPass::Execute
         */
        void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) override;

    private:
        /**
         * @brief Update the OpenGL settings prior to rendering
         * @param camera the camera to render with
         */
        void UpdateGLSettings(const Camera& camera) const;

        /**
         * @brief
         * @param camera
         * @param projection_matrix
         * @param view_matrix
         * @param volume
         */
        void RenderVolume(const Camera& camera,
                          const math::Matrix4x4& projection_matrix,
                          const math::Matrix4x4& view_matrix,
                          const Volume& volume);

        void RenderAnimatedModel(std::shared_ptr<Model> model,
                                 std::shared_ptr<IProgram> graphics_program,
                                 const Animator& animator,
                                 const TimeDelta& time_delta);
        void RenderModel(std::shared_ptr<Model> model);

        /**
         * @brief Setup the graphic program for rendering
         *
         * Sets all material, texture, and matrix uniforms
         *
         * @param graphics_program the graphics program
         * @param transform the transform component
         * @param material the material component
         * @param view_matrix the view matrix of the rendering camera
         */
        void SetupGraphicsProgram(std::shared_ptr<GLProgram> graphics_program,
                                  const Transform& transform,
                                  const Material& material,
                                  const math::Matrix4x4& view_matrix);

        /**
         * @brief Cleanup the graphics program
         *
         * Unbinds all textures from the texture units
         *
         * @param graphics_program the graphics program
         */
        void CleanupGraphicsProgram(std::shared_ptr<GLProgram> graphics_program);

        static void ToggleWireframeMode(bool enable_wireframe_mode);
        static void ToggleBackfaceCulling(bool enable_backface_culling);

        static const char* kTitle;

        GLCompiler* gl_compiler_;
        GLModelManager* gl_model_manager_;
        GLPrimitiveMeshManager* gl_primitive_mesh_manager_;
        GLTextureManager* gl_texture_manager_;
        GLUniformManager* gl_uniform_manager_;
        std::shared_ptr<GLSampler> shadow_map_sampler_;
        std::shared_ptr<GLSampler> diffuse_map_sampler_;

    }; // class GLEntityRenderPass

} // namespace zero::render