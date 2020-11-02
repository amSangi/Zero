#pragma once

#include "render/IRenderPass.hpp"
#include "core/Logger.hpp"
#include "component/Transform.hpp"
#include "component/Material.hpp"
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
    class GLSampler;
    class GLTextureManager;
    class GLUniformManager;

    class GLEntityRenderPass : public IRenderPass
    {
    public:
        GLEntityRenderPass(GLCompiler* gl_compiler,
                           GLModelManager* gl_model_manager,
                           GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                           GLTextureManager* gl_texture_manager,
                           GLUniformManager* gl_uniform_manager,
                           Logger& logger);
        ~GLEntityRenderPass() override = default;

        /**
         * @brief IRenderPass::Execute
         */
        void Execute(const Camera& camera, const entt::registry& registry) override;

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

        static void ToggleWireframeMode(bool enable_wireframe_mode);
        static void ToggleBackfaceCulling(bool enable_backface_culling);

        static const char* kTitle;

        GLCompiler* gl_compiler_;
        GLModelManager* gl_model_manager_;
        GLPrimitiveMeshManager* gl_primitive_mesh_manager_;
        GLTextureManager* gl_texture_manager_;
        GLUniformManager* gl_uniform_manager_;
        Logger& logger_;
        std::shared_ptr<GLSampler> shadow_map_sampler_;
        std::shared_ptr<GLSampler> diffuse_map_sampler_;

    }; // class GLEntityRenderPass

} // namespace zero::render