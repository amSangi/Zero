#pragma once

#include <string>
#include "core/ZBase.hpp"

// Forward declarations
class SDL_Surface;

namespace zero::render
{

    /**
     * @brief Stores and Removes Images from memory.
     *  Used to make SDL_Surface types given an image filename.
     */
    class Image
    {
    public:

        /**
         * @brief The pixel format of the data
         */
        enum class PixelFormat
        {
            UNDEFINED,  ///< Undefined pixel format
            BGR,        ///< BGR pixel format
            RGB,        ///< RGB pixel format
            RGBA,       ///< RGBA pixel format
            BGRA,       ///< BGRA pixel format
        }; // enum class PixelFormat

        /**
         * @brief Constructor
         * @param filename The fully qualified filename
         */
        explicit Image(std::string filename);

        ~Image();

        /**
         * @brief Load the image into memory.
         * @return True if the image was successfully loaded. Otherwise false.
         */
        bool Load();

        /**
         * @brief Releases the image from memory.
         * @return True if the image was successfully released. Otherwise false.
         */
        bool Release();

        /**
         * @brief Has the image been loaded into memory?
         * @return True if the image has been loaded. Otherwise false.
         */
        [[nodiscard]] bool IsLoaded() const;

        /**
         * @brief Get the image data.
         * @return the image data. nullptr if not loaded.
         */
        [[nodiscard]] char* GetData() const;

        /**
         * @brief Get the image width.
         * @return the image width. 0 if not loaded.
         */
        [[nodiscard]] uint32 GetWidth() const;

        /**
         * @brief Get the image height.
         * @return the image height. 0 if not loaded.
         */
        [[nodiscard]] uint32 GetHeight() const;

        /**
         * @brief Get the image pitch.
         * @return the image pitch. 0 if not loaded.
         */
        [[nodiscard]] uint32 GetPitch() const;

        /**
         * @brief Get the number of bits per pixel.
         * @return the bits per pixel. 0 if not loaded.
         */
        [[nodiscard]] uint8 BitsPerPixel() const;

        /**
         * @brief Get the number of bytes per pixel.
         * @return the bytes per pixel. 0 if not loaded.
         */
        [[nodiscard]] uint8 BytesPerPixel() const;

        /**
         * @brief Get the pixel format.
         * @return the pixel format. PIXEL_FORMAT_UNDEFINED if not loaded.
         */
        [[nodiscard]] PixelFormat GetPixelFormat() const;

    private:
        /**
         * @brief The fully qualified image filename
         */
        std::string filename_;

        /**
         * @brief The internal data container
         */
        SDL_Surface* surface_;

    }; // class Image

} // namespace zero::render