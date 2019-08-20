#pragma once

#include <entt/entt.hpp>
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
         * @brief Render all entities in the registry with Transform, Volume, and Material components using the
         *     active cameras in the world.
         * @param registry the registry containing all of the entities in the game world
         * @param dt the time between the current and last frame
         */
        virtual void Render(const entt::registry& registry, float dt) = 0;

        /**
         * @brief Perform post-rendering cleanup
         * @param registry the registry containing all of the entities in the game world
         */
        virtual void PostRender(entt::registry& registry) = 0;

        /**
         * @brief Clear all texture images, shaders, and models
         */
        virtual void ShutDown() = 0;
        
    }; // class IRenderer

} // namespace zero::render