#pragma once

#include <entt/entt.hpp>

namespace zero::render {

    /**
     * @brief Animate all dynamic models with keyframe data
     */
    class IAnimator {
    public:

        IAnimator() = default;

        virtual ~IAnimator() = default;

        /**
         * @brief Initialize all the animators sub systems
         */
        virtual void Initialize() = 0;

        /**
         * @brief Perform an animation tick
         * @param registry the registry containing all of the entities
         * @param dt the time between the current and last frame
         */
        virtual void Animate(entt::registry& registry, float dt) = 0;

    }; // class IAnimator

} // namespace zero::render