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
         * @brief Texture filtering techniques for texture sampling.
         *   For magnification filtering, only Nearest and Linear are used.
         */
        enum class Filter {
            NEAREST,                ///< Use the texel that is nearest to the centre of the pixel. The default filter.
            LINEAR,                 ///< Use the weighted average of the four nearest texels to the centre of the pixel
            NEAREST_MIPMAP_NEAREST, ///< Nearest mipmap with nearest neighbour interpolation of texels.
            LINEAR_MIPMAP_NEAREST,  ///< Nearest mipmap with linear interpolation of texels
            NEAREST_MIPMAP_LINEAR,  ///< Linear interpolation of the two closest mipmaps using nearest interpolation
            LINEAR_MIPMAP_LINEAR,   ///< Linear interpolation of the two closest mipmaps using linear interpolation
        }; // enum class Filter

        /**
         * @brief Wrapping techniques for texture sampling
         */
        enum class Wrapping {
            REPEAT,            ///< Coordinates will repeat in a repeating pattern. The default technique.
            CLAMP_TO_EDGE,     ///< Coordinates will be clamped to the respective edge
            CLAMP_TO_BORDER,   ///< Coordinates outside the range will use a provided border colour (default black)
            MIRRORED_REPEAT,   ///< Coordinates will repeat in a repeating mirror pattern
        }; // enum class Wrapping

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