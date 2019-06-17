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
         * @brief Texture filtering techniques for texture sampling
         */
        enum class Filter {
            NEAREST,                ///< Use the texel that is the nearest to the centre of the pixel
            LINEAR,                 ///< Use the weighted average of the four nearest texels to the centre of the pixel
            NEAREST_MIPMAP_NEAREST, ///< Nearest mipmap with nearest neighbour interpolation of texels
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

        /**
         * @brief Set the wrapping technique for the s coordinate in the coordinates (s, t, r).
         * @param s The wrapping technique
         */
        virtual void SetWrappingS(Wrapping s) = 0;

        /**
         * @brief Set the wrapping technique for the t coordinate in the coordinates (s, t, r).
         * @param t The wrapping technique
         */
        virtual void SetWrappingT(Wrapping t) = 0;

        /**
         * @brief Set the wrapping technique for the r coordinate in the coordinates (s, t, r).
         * @param r The wrapping technique
         */
        virtual void SetWrappingR(Wrapping r) = 0;

        /**
         * @brief Set the texture filtering techniques.
         * @param minification The texture minification technique to use
         * @param magnification The texture magnification technique to use
         */
        virtual void SetFilters(Filter minification, Filter magnification) = 0;

        /**
         * @brief Generate MipMaps
         */
        virtual void GenerateMipMaps() = 0;

        /**
         * @brief Set the border colour.
         * @param colour The colour.
         */
        virtual void SetBorderColour(math::Vec3f colour) = 0;

    }; // class ITexture

} // namespace zero::render