#pragma once

#include <string>
#include "core/Component.hpp"
#include "core/ZBase.hpp"

namespace zero::render {

    /**
     * @brief An instance of a 3D model.
     */
    struct ModelInstance : public Component {

        /**
         * @brief The fully qualified filename of the 3D model.
         */
        std::string filename_;

        /**
         * @brief Identifier for a child of a 3D model. 0 for root models.
         */
        uint32 child_identifier_;

    }; // struct ModelInstance

} // namespace zero::render
