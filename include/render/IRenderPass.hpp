#pragma once

#include <entt/entt.hpp>
#include "component/Camera.hpp"
#include "core/TimeDelta.hpp"

namespace zero::render
{

    /**
     * @brief Encapsulates a rendering step
     *
     * A rendering step can include:
     * - Rendering the sky dome
     * - Creating shadow maps
     * - Rendering the non-culled entities with a mesh to the color buffer
     * - Multiple draw calls
     */
    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;

        /**
         * @brief Execute the rendering pass
         *
         * @param camera the camera to render to
         * @param registry the registry containing all the game entities and their components
         * @param time_delta updated timing information since the last engine tick
         */
        virtual void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) = 0;

    }; // interface IRenderPass

} // namespace zero::render