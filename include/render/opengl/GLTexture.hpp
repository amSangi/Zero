#pragma once

#include <GL/glew.h>
#include <memory>
#include <SDL_opengl.h>
#include "render/Image.hpp"
#include "render/ITexture.hpp"

namespace zero::render {

    class GLTexture : public ITexture {
    public:

        GLTexture(GLuint id, GLenum target);

        ~GLTexture() override;

        void Use(GLenum texture);

    protected:
        void Cleanup() override;

    private:
        GLuint id_;
        GLenum target_;

    }; // class GLTexture

    /**
     * @brief GLTexture builder.
     */
    class GLTextureBuilder {
    public:

        /**
         * @brief Create a GLTextureBuilder using the given image and GL target texture
         * @param image the image the texture will use
         * @param target the GL target texture
         */
        GLTextureBuilder(std::shared_ptr<Image> image, GLenum target);

        /**
         * @brief Default destructor.
         */
        ~GLTextureBuilder() = default;

        /**
         * @brief Set the wrapping technique for the s texture coordinate axis.
         * @param s the texture coordinate wrapping technique
         * @return the builder
         */
        GLTextureBuilder& SetWrappingS(ITexture::Wrapping s);

        /**
         * @brief Set the wrapping technique for t texture coordinate axis.
         * @param t the texture coordinate wrapping technique
         * @return the builder
         */
        GLTextureBuilder& SetWrappingT(ITexture::Wrapping t);

        /**
         * @brief Set the minification texture filtering technique.
         * @param filter the minification texture filter technique
         * @return the builder
         */
        GLTextureBuilder& SetMinificationFilter(ITexture::Filter filter);

        /**
         * @brief Set the magnification texture filtering technique. NEAREST and LINEAR are the only used techniques
         *   magnification filtering.
         * @param filter the magnification texture filter technique
         * @return the builder
         */
        GLTextureBuilder& SetMagnificationFilter(ITexture::Filter filter);

        /**
         * @brief Set the clamp border colour. Only used if CLAMP_TO_BORDER wrapping is used.
         * @param colour the border colour
         * @return the builder
         */
        GLTextureBuilder& SetBorderColour(math::Vec3f colour);

        /**
         * @brief Build the GLTexture
         * @return a GLTexture. nullptr if the build failed.
         */
        std::shared_ptr<GLTexture> Build();

    private:

        /**
         * @brief The image containing the texture data
         */
        std::shared_ptr<Image> image_;

        /**
         * @brief The texture target
         */
        GLenum target_;

        /**
         * @brief The <s,t> texture wrapping techniques
         */
        ITexture::Wrapping wrappings_[2];

        /**
         * @brief The minification filter to use
         */
        ITexture::Filter min_filter_;

        /**
         * @brief The magnification filter to use
         */
        ITexture::Filter mag_filter_;

        /**
         * @brief The border colour used by the CLAMP_TO_BORDER wrapping
         */
        math::Vec3f border_colour_;

    }; // class GLTextureBuilder

} // namespace zero::render