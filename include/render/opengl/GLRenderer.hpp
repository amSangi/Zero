#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "core/TimeDelta.hpp"
#include "core/Transform.hpp"
#include "render/IRenderer.hpp"
#include "render/IShader.hpp"

namespace zero::render {

    // Forward declarations
    class Camera;
    class GLCompiler;
    class GLModelManager;
    class GLPrimitiveMeshManager;
    class GLTextureManager;
    class IProgram;
    class Volume;

    /**
     * @brief Render all drawable entities in the game using OpenGL.
     * An entity is considered drawable if it contains Transform, Material, Volume, and Mesh components.
     * Entities with the Camera component are used to perform the rendering.
     */
    class GLRenderer final : public IRenderer {
    public:

        GLRenderer();

        ~GLRenderer() override;

        /**
         * @brief Load all OpenGL shaders, models, and textures
         * @param config the configuration containing the respective assets
         */
        void Initialize(const RenderSystemConfig& config) override;

        /**
         * @see IRenderer::Render
         */
        void Render(const entt::registry& registry) override;

        /**
         * @brief Release the images in the TextureManager from main memory
         * @param registry the registry containing all the entities and their components
         */
        void PostRender(entt::registry& registry) override;

        /**
         * @brief Clear the Compiler of shaders, the ModelManager of models, and TextureManager of textures
         */
        void ShutDown() override;

        /**
         * @see IRenderer::InstantiateModel
         */
        Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) override;

        /**
         * @see IRenderer::InstantiatePrimitive
         */
        Component::Entity InstantiatePrimitive(entt::registry& registry, const PrimitiveInstance& primitive) override;

    private:
        /**
         * @brief Initialization helper
         */
        ///@{
        void InitializeShaders(const RenderSystemConfig& config);
        void InitializeShaderFiles(const std::vector<std::string>& shaders, IShader::Type shader_type);
        void InitializeModels(const RenderSystemConfig& config);
        void InitializeImages(const RenderSystemConfig& config);
        ///@}

        /**
         * @brief Render all viewable entities with a mesh
         * @param camera the camera to render to
         * @param registry the registry containing all the entities and their components
         */
        void RenderEntities(const Camera& camera, const entt::registry& registry);

        /**
         * @brief Render the wireframe of the bounding volume
         * @param projection_matrix the projection matrix of the camera
         * @param view_matrix the view matrix of the camera
         * @param volume the volume to render
         */
        void RenderVolume(const math::Matrix4x4& projection_matrix,
                          const math::Matrix4x4& view_matrix,
                          const Volume& volume);

        /**
         * @brief Initialize OpenGL for rendering (e.g. enable depth testing)
         */
        static void InitializeGL();

        /**
         * @brief Update all OpenGL settings (e.g. Set viewport, clear color buffers, etc)
         * @param camera the camera being rendered to
         */
        static void UpdateGL(const Camera& camera);

        /**
         * @brief Toggle wireframe mode
         * @param enable_wireframe should wireframe be enabled?
         */
        static void ToggleWireframeMode(bool enable_wireframe);

        /**
         * @brief Set the light uniforms in the shader program
         *
         * Queries the registry for all entities with Transform and Light components
         *
         * @param graphics_program the shader program
         * @param registry the registry containing all the entities and their components
         */
        static void SetLightUniforms(std::shared_ptr<IProgram> graphics_program, const entt::registry& registry);

        /**
         * @brief Get all non-culled entities that are renderable.
         *
         * An entity is viewable if it is not culled by the camera and if it contains either a Model mesh or Primitive
         * mesh.
         * Required components: Transform, Material, Volume, (ModelInstance or PrimitiveInstance)
         *
         * @param registry the registry containing all the entities and their components
         * @param camera the camera render to
         * @return all viewable entities
         */
        static std::vector<Component::Entity> GetViewableEntities(const entt::registry& registry,
                                                                  const Camera& camera);

        /**
         * @brief Read the shader source into the destination
         * @param filename the shader file
         * @param destination the destination
         */
        static void ReadShaderSource(const std::string& filename, std::string& destination);

        /**
         * @brief Manage shader and graphics program usage
         */
        std::unique_ptr<GLCompiler> graphics_compiler_;

        /**
         * @brief Manage model meshes
         */
        std::unique_ptr<GLModelManager> model_manager_;
        
        /**
         * @brief Manage primitive meshes
         */
        std::unique_ptr<GLPrimitiveMeshManager> primitive_manager_;

        /**
         * @brief Manage texture image usage
         */
        std::unique_ptr<GLTextureManager> texture_manager_;

    }; // class GLRenderer

} // namespace zero::render