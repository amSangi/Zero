#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "core/Transform.hpp"
#include "render/IRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
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
         * @param dt time between the current and last frame
         */
        void Render(const entt::registry& registry, float dt) override;

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
         * Constructs an entity with a Transform, Volume, Material, and MeshInstance components.
         *
         * @param registry the registry containing all the entities and their components
         * @param model the fully qualified 3D model file to instantiate
         */
        Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) override;

    private:

        /**
         * @brief Render an entity using the given camera
         * @param camera the camera to use for the render
         * @param transform the transform of the entity
         * @param material the material of the entity
         * @param mesh_instance the mesh instance of the entity
         */
         void RenderEntity(const Camera& camera,
                           const Transform& transform,
                           const Material& material,
                           const MeshInstance& mesh_instance);

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
         * @brief Texture sampler used for all textures
         */
        std::shared_ptr<GLSampler> texture_sampler_;

    }; // class GLRenderer

} // namespace zero::render