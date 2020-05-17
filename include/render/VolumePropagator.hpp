#pragma once

#include <entt/entt.hpp>

namespace zero::render
{

    /**
     * @brief Propagates Volume information to child/parent components
     */
    class VolumePropagator
    {
    public:

        VolumePropagator() = delete;

        /**
         * @brief Propagate bounding volume up and expand/contract
         * @param registry the registry containing the entities and their components
         */
        static void PropagateVolume(entt::registry& registry);

    }; // class VolumePropagator

} // namespace zero::render