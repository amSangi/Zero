#pragma once

#include <memory>
#include <string>
#include "ITexture.hpp"
#include "Components.hpp"

namespace zero::render {

    class ITextureManager {
    public:

        /**
         * @brief Default constructor
         */
        ITextureManager() = default;

        /**
         * @brief Default virtual destructor
         */
        virtual ~ITextureManager() = default;

        /**
         * @brief Get the maximum number of texture units
         * @return the texture unit count
         */
        virtual uint8 GetTextureUnitCount() const = 0;

        /**
         * @brief Initialize an image
         * @param filename the image file name
         * @return True if the image was initialized successfully. Otherwise false.
         */
        virtual bool InitializeImage(const std::string& filename) = 0;

        /**
         * @brief Create a graphics texture given a Material component
         * @param filename the image file name
         * @param index the texture index. Range from [0, GetTextureUnitCount).
         * @return a graphics texture. nullptr if the image has not been initialized.
         */
        std::shared_ptr<ITexture> CreateTexture(const std::string& filename, uint8 index)
        { return std::shared_ptr<ITexture>(CreateRawTexture(filename, index)); }

    protected:

        /**
         * @brief Create a graphics texture given a Material component
         * @param filename the image file name
         * @param index the texture index. Range from [0, GetTextureUnitCount).
         * @return a graphics texture. nullptr if the image has not been initialized.
         */
        virtual ITexture* CreateRawTexture(const std::string& filename, uint8 index) = 0;

    }; // class ITextureManager

} // namespace zero::render