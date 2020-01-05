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
        void Render(const entt::registry& registry, const TimeDelta& time_delta) override;

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

        /**
         * @brief Render an entity
         * @param registry the registry containing all the entities and their components
         * @param viewable_entities the list of all non-culled entities
         * @param graphics_program the graphics program used for the rendering of the entity
         * @param view_matrix the camera's view matrix
         * @param projection_matrix the camera's projection matrix
         * @param transform the transform of the entity
         * @param model the 3D model associated with the entity
         */
        static void RenderEntity(const entt::registry& registry,
                                 const std::vector<Component::Entity>& viewable_entities,
                                 const std::shared_ptr<IProgram>& graphics_program,
                                 const math::Matrix4x4& view_matrix,
                                 const math::Matrix4x4& projection_matrix,
                                 const Transform& transform,
                                 const std::shared_ptr<GLModel>& model);

        /**
         * @brief Read the shader source into the destination
         * @param filename the shader file
         * @param destination the destination
         */
        void ReadShaderSource(const std::string& filename, std::string& destination) const;

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