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
         * @brief Load the image data into main memory
         * @param image_name the name of the image
         * @param filename the fully qualified filename of the image
         * @return True if the image was loaded successfully. Otherwise false.
         */
        bool InitializeImage(const std::string& image_name, const std::string& filename);

        /**
         * @brief Load the already loaded images onto the graphics card
         * @return True if the images were loaded successfully. Otherwise false.
         */
        bool InitializeGLTextures();

        /**
         * @brief Return the GLTexture for an image
         * @param image_name the name of the image
         * @return the GLTexture. Nullptr if it has not been loaded.
         */
        std::shared_ptr<GLTexture> GetGLTexture(const std::string& image_name) const;

        /**
         * @brief Return the list of shadow map textures
         * @return the shadow map GLTextures
         */
        const std::vector<std::shared_ptr<GLTexture>>& GetShadowMapTextures() const;

        /**
         * @brief Set the shadow map GLTextures
         * @param texture the shadow map textures
         */
        void SetShadowMapTextures(const std::vector<std::shared_ptr<GLTexture>>& textures);

        /**
         * @brief Unload the image data from main memory
         */
        void UnloadImages();

        /**
         * @brief Unload the GLTextures from the graphics card
         */
        void UnloadGLTextures();

    private:

        /**
         * @brief Create a graphics texture object
         * @param image the image to use for the graphics texture
         * @return an OpenGL graphics texture object
         */
        std::shared_ptr<GLTexture> CreateTexture(std::shared_ptr<Image> image);

        /**
         * @brief Image name to Image map
         */
        std::unordered_map<std::string, std::shared_ptr<Image>> image_map_;

        /**
         * @brief Image name to GLTexture map
         */
        std::unordered_map<std::string, std::shared_ptr<GLTexture>> gl_texture_map_;

        /**
         * @brief The shadow map textures
         */
        std::vector<std::shared_ptr<GLTexture>> shadow_map_textures_;

        /**
         * @brief A 1x1 black 2D texture used for unused sampler2Ds
         */
        std::shared_ptr<GLTexture> empty_texture_;

    }; // class GLTextureManager

} // namespace render
} // namespace zero