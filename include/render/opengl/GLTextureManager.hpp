#pragma once

#include <memory>
#include <unordered_map>
#include "render/ITextureManager.hpp"
#include "render/Image.hpp"

namespace zero::render {

    // Forward Declarations
    class GLTexture;
    class GLSampler;

    /**
     * @brief Create OpenGL texture objects and manage image lifetime
     */
    class GLTextureManager : public ITextureManager {
    public:

        GLTextureManager();

        ~GLTextureManager() override = default;

        /**
         * @see ITextureManager::GetTextureUnitCount
         */
        uint8 GetTextureUnitCount() const override;

        /**
         * @brief Set the texture sampler for a given texture unit
         * @param sampler the texture sampler to assign
         * @param texture_unit the texture unit to assign the sampler to. Range from [0, GetTextureUnitCount).
         */
        void SetSampler(const std::shared_ptr<GLSampler>& sampler, uint8 texture_unit);

        /**
         * @brief Create a graphics texture object
         * @param filename the filename of the image to use for the graphics texture
         * @param index the texture unit to use (0 based)
         * @return an OpenGL graphics texture object
         */
        std::shared_ptr<GLTexture> CreateTexture(const std::string& filename, uint8 index);

        /**
         * @see ITextureManager::InitializeImage
         */
        bool InitializeImage(const std::string& filename) override;

        /**
         * @brief Clear the image map
         */
        void ClearImages();

        /**
         * @brief Release the images from main memory
         */
        void UnloadImages();

    private:

        /**
         * @brief Filename to Image container
         */
        std::unordered_map<std::string, std::shared_ptr<Image>> image_map_;

    }; // class GLTextureManager

} // namespace zero::render