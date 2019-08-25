#pragma once

#include <string>
#include "core/ZBase.hpp"

namespace zero::render {

    /**
     * @brief Stores Image objects that can be loaded into Main Memory to be used to create graphics Texture objects.
     * The Material component dictates the textures used during the rendering of an entity.
     */
    class ITextureManager {
    public:

        ITextureManager() = default;

        virtual ~ITextureManager() = default;

        /**
         * @brief Get the maximum number of texture units
         * @return the texture unit count
         */
        [[nodiscard]] virtual uint8 GetTextureUnitCount() const = 0;

        /**
         * @brief Initialize an image
         * @param filename the image file name
         * @return True if the image was initialized successfully. Otherwise false.
         */
        virtual bool InitializeImage(const std::string& filename) = 0;

    }; // class ITextureManager

} // namespace zero::render