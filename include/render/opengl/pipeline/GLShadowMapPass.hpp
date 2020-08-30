#pragma once

#include <array>
#include "render/IRenderPass.hpp"
#include "render/opengl/OpenGL.hpp"
#include "render/opengl/GLTexture.hpp"
#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/Transform.hpp"
#include "math/Matrix4x4.hpp"

namespace zero::render
{
    // Forward declarations
    class GLCompiler;
    class GLModelManager;
    class GLPrimitiveMeshManager;
    class GLTextureManager;
    class GLUniformManager;

    class GLShadowMapPass : public IRenderPass
    {
    public:
        GLShadowMapPass(GLCompiler* gl_compiler,
                        GLModelManager* gl_model_manager,
                        GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                        GLTextureManager* gl_texture_manager,
                        GLUniformManager* gl_uniform_manager,
                        uint32 width,
                        uint32 height);
        ~GLShadowMapPass() override;

        void Initialize();
        void Execute(const Camera& camera,
                     entt::registry& registry,
                     const std::vector<Entity>& viewable_entities) override;

    private:
        void InitializeTexture();
        void InitializeFrameBufferObject();
        void RenderEntities(const math::Matrix4x4& light_view_matrix,
                            entt::registry& registry,
                            const std::vector<Entity>& viewable_entities);

        GLCompiler* gl_compiler_;
        GLModelManager* gl_model_manager_;
        GLPrimitiveMeshManager* gl_primitive_mesh_manager_;
        GLTextureManager* gl_texture_manager_;
        GLUniformManager* gl_uniform_manager_;
        uint32 shadow_map_width_;
        uint32 shadow_map_height_;
        GLuint fbo_id_;
        std::shared_ptr<GLTexture> shadow_map_texture_;

    }; // class GLShadowMapPass

} // namespace zero::render