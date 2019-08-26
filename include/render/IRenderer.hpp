#pragma once

#include <entt/entt.hpp>
#include "render/Components.hpp"
#include "render/RenderSystemConfig.hpp"

namespace zero::render {

    /**
     * @brief Draw primitives and objects onto the current active rendering context
     */
    class IRenderer {
    public:

        IRenderer() = default;

        virtual ~IRenderer() = default;

        /**
         * @brief Initialize all the texture images, shaders, and models
         * @param config the render system configuration details
         */
        virtual void Initialize(const RenderSystemConfig& config) = 0;

        /**
         * @brief Render all entities in the registry with Transform, Volume, Material, and MeshInstance components
         * using the active cameras in the world.
         * @param registry the registry containing all of the entities and their components
         * @param dt the time between the current and last frame
         */
        virtual void Render(const entt::registry& registry, float dt) = 0;

        /**
         * @brief Perform post-rendering cleanup
         * @param registry the registry containing all of the entities and their components
         */
        virtual void PostRender(entt::registry& registry) = 0;

        /**
         * @brief Clear all texture images, shaders, and models
         */
        virtual void ShutDown() = 0;

        /**
         * @brief Create a new entity based on a 3D model.
         *
         * Constructs an entity with a Transform, Volume, Material, and MeshInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        virtual Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) = 0;
        
    }; // class IRenderer

} // namespace zero::render