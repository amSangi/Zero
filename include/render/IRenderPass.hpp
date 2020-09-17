#pragma once

#include <entt/entt.hpp>
#include "component/Camera.hpp"

namespace zero::render
{

    /**
     * @brief Encapsulates a rendering step
     *
     * A rendering step can include:
     * - Rendering the sky dome
     * - Creating shadow maps
     * - Rendering the non-culled entities with a mesh to the color buffer
     */
    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;

        /**
         * @brief Execute the rendering pass
         *
         * The list of viewable entities contains the list of all entities in the world that have not been culled.
         * These entities include the following components
         * - Transform
         * - Material
         * - Volume
         * - PrimitiveInstance or ModelInstance
         *
         * @param camera the camera to render to
         * @param registry the registry containing all the game entities and their components
         * @param viewable_entities the list of all viewable entities
         */
        virtual void Execute(const Camera& camera,
                             entt::registry& registry,
                             const std::vector<Entity>& viewable_entities) = 0;
    }; // interface IRenderPass

} // namespace zero::render