#pragma once

#include <entt.hpp>
#include "math/Matrix4x4.hpp"
#include "render/Components.hpp"

namespace zero::render {

    /**
     * @brief Propagates data from parent entities to their child entities
     */
    class Propagator {
    public:

        /**
         * @brief Constructor
         */
        Propagator() = default;

        /**
         * @brief Propagate transform data and update bounding volume position
         * @param registry the registry containing the entities and their components
         */
        void PropagateTransform(entt::registry& registry) const;

        /**
         * @brief Propagate bounding volume up and expand/contract
         * @param registry the registry containing the entities and their components
         */
        void PropagateVolume(entt::registry& registry) const;

    private:

        /**
         * @brief Apply the affine transformation to the bounding volume
         * @param volume the volume to transform
         * @param transformation the affine transformation
         */
        void TransformVolume(Volume& volume, const zero::math::Matrix4x4& transformation) const;

    }; // class Propagator

} // namespace zero::render