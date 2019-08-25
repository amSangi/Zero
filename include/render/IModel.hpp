#pragma once

#include "IRenderResource.hpp"
#include "Components.hpp"
#include "math/Matrix4x4.hpp"

namespace zero::render {

    /**
     * @brief Base class for 3D render models
     */
    class IModel : public IRenderResource {
    public:

        IModel() = default;

        virtual ~IModel() = default;

        /**
         * @brief Get the transformation matrix relative to the parent
         * @return the transformation matrix
         */
        [[nodiscard]] virtual math::Matrix4x4 GetTransformation() const = 0;

        /**
         * @brief Get a material associated with the model
         * @return a material component prototype instance
         */
        [[nodiscard]] virtual Material GetMaterial() const = 0;

        /**
         * @brief Get a volume associated with the model
         * @return a volume component prototype instance
         */
        [[nodiscard]] virtual Volume GetVolume() const = 0;

    }; // class IModel

} // namespace zero::render