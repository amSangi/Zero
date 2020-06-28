#pragma once

#include <string>
#include "component/Component.hpp"
#include "core/ZeroBase.hpp"

namespace zero
{

    /**
     * @brief An instance of a 3D model.
     */
    struct ModelInstance : public Component
    {

        /**
         * @brief The name of the model
         */
        std::string model_name_;

        /**
         * @brief Identifier for a child of a 3D model. 0 for root models.
         */
        uint32 child_identifier_;

    }; // struct ModelInstance

} // namespace zero
