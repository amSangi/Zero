#pragma once

#include "render/IRenderPass.hpp"
#include "component/SkyDome.hpp"

namespace zero::render
{
    // Forward declarations
    class GLCompiler;
    class GLMesh;
    class GLPrimitiveMeshManager;
    class GLUniformManager;

    class GLSkyDomePass : public IRenderPass
    {
    public:
        GLSkyDomePass(GLCompiler* gl_compiler,
                      GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                      GLUniformManager* gl_uniform_manager);
        ~GLSkyDomePass() override = default;

        void Execute(const Camera& camera,
                     entt::registry& registry,
                     const std::vector<Entity>& viewable_entities) override;

    private:
        void RenderSkyDome(const Camera& camera, const SkyDome& sky_dome);

        std::shared_ptr<GLMesh> gl_sphere_;
        GLCompiler* gl_compiler_;
        GLPrimitiveMeshManager* gl_primitive_mesh_manager_;
        GLUniformManager* gl_uniform_manager_;

    }; // class GLSkyDomePass

} // namespace zero::render