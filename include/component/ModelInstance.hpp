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
         * @brief The name of the model node.
         *
         * A 3D model may contain multiple sub-models with different node names
         */
        std::string node_name_;

    }; // struct ModelInstance

} // namespace zero
