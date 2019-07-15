#pragma once

#include "Components.hpp"

namespace zero::render {

    /**
     * @brief Culling interface for different culling techniques
     */
    class ICuller {
    public:

        /**
         * @brief Constructor
         */
        ICuller() = default;

        /**
         * @brief Destructor
         */
        virtual ~ICuller() = default;

        /**
         * @brief Should the volume be culled?
         * @param volume the volume to cull
         * @return true if the volume should be culled. Otherwise false.
         */
        virtual bool IsCulled(const Volume& volume) const = 0;

    }; // class ICuller

} // namespace zero::render