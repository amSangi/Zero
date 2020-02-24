#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "core/TimeDelta.hpp"
#include "core/Transform.hpp"
#include "render/IProgram.hpp"
#include "render/IRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLInstantiator.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLTextureManager.hpp"

namespace zero::render {

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
         * @brief Render all drawable entities in the game world
         * @param registry the registry containing all the entities and their components
         * @param time_delta updated timing information since the last engine tick
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
         * @brief Create an entity instance based on a 3D model
         *
         * Constructs an entity with a Transform, Volume, Material, and ModelInstance components.
         *
         * @param registry the registry containing all the entities and their components
         * @param model the fully qualified 3D model file to instantiate
         */
        Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) override;

    private:

        void InitializeShaders(const RenderSystemConfig& config);
        void InitializeShaderFiles(const std::vector<std::string>& shaders, IShader::Type shader_type);
        void InitializeModels(const RenderSystemConfig& config);
        void InitializeImages(const RenderSystemConfig& config);
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
         * @brief Render the entities using the camera
         * @param camera the camera to render with
         * @param registry the registry containing all the entities and their components
         */
        void RenderWithCamera(const Camera& camera,
                              const entt::registry& registry);

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
         * @brief Get all non-culled entities that are renderable.
         * @param registry the registry containing all the entities and their components
         * @param camera the camera to cull against
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
         * @brief Manage mesh usage
         */
        std::unique_ptr<GLModelManager> model_manager_;

        /**
         * @brief Manage texture image usage
         */
        std::unique_ptr<GLTextureManager> texture_manager_;

        /**
         * @brief Instantiate game entities from GLModels
         */
        std::unique_ptr<GLInstantiator> instantiator_;

    }; // class GLRenderer

} // namespace zero::render