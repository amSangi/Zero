#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "core/ZeroBase.hpp"

namespace zero
{
    // Forward declarations
    class Material;

namespace render
{

    // Forward Declarations
    class GLTexture;
    class GLSampler;
    class Image;

    /**
     * @brief Create OpenGL texture objects and manage image lifetime
     */
    class GLTextureManager
    {
    public:

        GLTextureManager();

        ~GLTextureManager() = default;

        /**
         * @brief Get the maximum number of texture units
         * @return the texture unit count
         */
        uint8 GetTextureUnitCount() const;

        /**
         * @brief Set the texture sampler for a given texture unit
         * @param sampler the texture sampler to assign
         * @param texture_unit the texture unit to assign the sampler to. Range from [0, GetTextureUnitCount).
         */
        void SetSampler(const std::shared_ptr<GLSampler>& sampler, uint8 texture_unit) const;

        /**
         * @brief Create a series of
         * @param material the material containing the texture map files
         * @return a list of GLTextures
         */
        std::vector<std::shared_ptr<GLTexture>> CreateTextureMap(const Material& material);

        /**
         * @brief Create a graphics texture object
         * @param image_name the name of the image to use for the texture
         * @param index the texture unit to use (0 based)
         * @param name the GLSL sampler2D property suffix name the texture is associated with.
         * e.g. If GLSL uniform is `material.diffuse_texture`, the name would be `diffuse_texture`
         * @return an OpenGL graphics texture object
         */
        std::shared_ptr<GLTexture> CreateTexture(const std::string& image_name, uint8 index, const std::string& name = "");

        /**
         * @brief Initialize an image
         * @param image_name the name of the image
         * @param filename the fully qualified filename of the image
         * @return True if the image was loaded successfully. Otherwise false.
         */
        bool InitializeImage(const std::string& image_name, const std::string& filename);

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
         * @brief Image name to Image map
         */
        std::unordered_map<std::string, std::shared_ptr<Image>> image_map_;

    }; // class GLTextureManager

} // namespace render
} // namespace zero