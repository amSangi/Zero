#pragma once

#include <string>
#include <entt/entt.hpp>
#include "core/RenderSystemConfig.hpp"

namespace zero::render {

    // Forward declarations
    class PrimitiveInstance;

    /**
     * @brief Draw primitives and objects onto the current active rendering context
     */
    class IRenderer {
    public:

        virtual ~IRenderer() = default;

        /**
         * @brief Initialize all the texture images, shaders, and models
         * @param config the render system configuration details
         */
        virtual void Initialize(const RenderSystemConfig& config) = 0;

        /**
         * @brief Render all entities in the registry with Transform, Volume, Material, and either
         * ModelInstance or PrimitiveInstance components using the active cameras in the world.
         * @param registry the registry containing all of the entities and their components
         */
        virtual void Render(const entt::registry& registry) = 0;

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
         * @brief Instantiate a 3D model.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        virtual Component::Entity InstantiateModel(entt::registry& registry, const std::string& model) = 0;

        /**
         * @brief Instantiate a new primitive.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        virtual Component::Entity InstantiatePrimitive(entt::registry& registry, const PrimitiveInstance& primitive) = 0;
        
    }; // class IRenderer

} // namespace zero::render