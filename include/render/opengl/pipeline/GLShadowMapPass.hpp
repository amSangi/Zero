#pragma once

#include <vector>
#include "render/IRenderPass.hpp"
#include "render/CascadedShadowMap.hpp"
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

        /**
         * @brief Setup GL variables (i.e. textures and frame buffer objects)
         */
        void Initialize();

        /**
         * @brief IRenderPass::Execute
         */
        void Execute(const Camera& camera,
                     entt::registry& registry,
                     const std::vector<Entity>& viewable_entities) override;

    private:
        /**
         * @brief Setup the shadow map textures
         */
        void InitializeTextures();

        /**
         * @brief Setup the frame buffer objects
         */
        void InitializeFrameBufferObjects();

        /**
         * @brief Update OpenGL settings prior to rendering
         */
        void UpdateGLSettings() const;

        /**
         * @brief Set the shadow casting directional light
         * @param registry the registry containing all the game entities and their components
         * @param out_directional_light the directional light to set
         * @return True if a shadow casting directional light exists in the game. Otherwise false.
         */
        static bool GetShadowCastingDirectionalLight(entt::registry& registry, DirectionalLight& out_directional_light);

        /**
         * @brief Render all non-culled viewable entities onto the shadow map
         * @param light_view_matrix the light view matrix
         * @param registry the registry containing all the game entities and their components
         * @param viewable_entities the list of non-culled viewable entities
         */
        void RenderEntities(const math::Matrix4x4& light_view_matrix, entt::registry& registry, const std::vector<Entity>& viewable_entities);

        static const uint32 kShadowCascadeCount;
        GLCompiler* gl_compiler_;
        GLModelManager* gl_model_manager_;
        GLPrimitiveMeshManager* gl_primitive_mesh_manager_;
        GLTextureManager* gl_texture_manager_;
        GLUniformManager* gl_uniform_manager_;
	    CascadedShadowMap cascaded_shadow_map_;
        uint32 shadow_map_width_;
        uint32 shadow_map_height_;
        std::vector<GLuint> fbo_ids_;
        std::vector<std::shared_ptr<GLTexture>> shadow_map_textures_;

    }; // class GLShadowMapPass

} // namespace zero::render