#pragma once

#include <entt.hpp>

namespace zero::render {

    /**
     * @brief Draw primitives and objects onto the current active rendering context
     */
    class IRenderer {
    public:
        /**
         * @brief Constructor
         */
        IRenderer() = default;

        /**
         * @brief Destructor
         */
        virtual ~IRenderer() = default;

        /**
         * @brief Initialize the renderer and all sub systems
         */
        virtual void Initialize() = 0;

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
         * @brief Shutdown the renderer and all of its sub-systems
         */
        virtual void ShutDown() = 0;
        
    }; // class IRenderer

} // namespace zero::render