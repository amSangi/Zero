#pragma once

#include "Components.hpp"
#include "core/Transform.hpp"

namespace zero::render {

    /**
     * @brief Base class for 3D render models
     */
    class IModel {
    public:

        virtual ~IModel() = default;

        /**
         * @brief Get a transform associated with the model
         * @return a prototype of a transform component
         */
        [[nodiscard]] virtual Transform GetTransform() const = 0;

        /**
         * @brief Get a material associated with the model
         * @return a prototype of a material component
         */
        [[nodiscard]] virtual Material GetMaterial() const = 0;

        /**
         * @brief Get a volume associated with the model
         * @return a prototype of a volume component
         */
        [[nodiscard]] virtual Volume GetVolume() const = 0;

        /**
         * @brief Get a model instance associated with the model
         * @return a prototype of a model instance component
         */
        [[nodiscard]] virtual ModelInstance GetModelInstance() const = 0;

    }; // class IModel

} // namespace zero::render