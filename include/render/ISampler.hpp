#pragma once

#include "math/Vector4.hpp"
#include "core/NonCopyable.hpp"

namespace zero::render {

    /**
     * @brief An ISampler describes how graphics textures should be sampled
     */
    class ISampler : public NonCopyable
    {
    public:

        /**
         * @brief Texture filtering techniques for texture sampling.
         *   For magnification filtering, only Nearest and Linear are used.
         */
        enum class Filter
        {
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
        enum class Wrapping
        {
            REPEAT,            ///< Coordinates will repeat in a repeating pattern. The default technique.
            CLAMP_TO_EDGE,     ///< Coordinates will be clamped to the respective edge
            CLAMP_TO_BORDER,   ///< Coordinates outside the range will use a provided border colour (default black)
            MIRRORED_REPEAT,   ///< Coordinates will repeat in a repeating mirror pattern
        }; // enum class Wrapping


        /**
         * @brief Default destructor.
         */
        virtual ~ISampler() = default;

        /**
         * @brief Initialize the sampler
         */
        virtual void Initialize() = 0;

        /**
         * @brief Set the wrapping technique for the s texture coordinate axis.
         * @param wrapping the texture coordinate wrapping technique
         */
        virtual void SetWrappingS(Wrapping wrapping) = 0;

        /**
         * @brief Set the wrapping technique for t texture coordinate axis.
         * @param wrapping the texture coordinate wrapping technique
         */
        virtual void SetWrappingT(Wrapping wrapping) = 0;

        /**
         * @brief Set the wrapping technique for r texture coordinate axis.
         * @param wrapping the texture coordinate wrapping technique
         */
        virtual void SetWrappingR(Wrapping wrapping) = 0;

        /**
         * @brief Set the minification texture filtering technique.
         * @param filter the minification texture filter technique
         */
        virtual void SetMinificationFilter(Filter filter) = 0;

        /**
         * @brief Set the magnification texture filtering technique. NEAREST and LINEAR are the only used techniques
         *   magnification filtering.
         * @param filter the magnification texture filter technique
         */
        virtual void SetMagnificationFilter(Filter filter) = 0;

        /**
         * @brief Set the clamp border colour. Only used if CLAMP_TO_BORDER wrapping is used.
         * @param colour the border colour
         */
        virtual void SetBorderColour(math::Vec4f colour) = 0;

    }; // class ISampler

} // namespace zero::render