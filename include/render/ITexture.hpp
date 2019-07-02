#pragma once

#include <string>
#include "math/Vector3.hpp"
#include "IRenderResource.hpp"
#include "Image.hpp"

namespace zero::render {

    /**
     * @brief A Graphics Texture object
     */
    class ITexture : public IRenderResource {
    public:

        /**
         * @brief Default Constructor
         */
        ITexture() = default;

        /**
         * @brief Virtual Destructor
         */
        ~ITexture() override = default;

    }; // class ITexture

} // namespace zero::render