#pragma once

#include <memory>
#include <entt/entt.hpp>
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
    class GLRenderer : public IRenderer {
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
         * @param registry the registry containing all the entities and their components
         * @param model the fully qualified 3D model file to instantiate
         */
        Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) override;

    private:

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

    }; // class GLRenderer

} // namespace zero::render