#pragma once

#include "render/renderer/ITexture.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL graphics texture wrapper
     */
    class GLTexture final : public ITexture
    {
    public:

        /**
         * @brief Constructor
         * @param id the texture object identifier
         * @param target the texture target
         */
        GLTexture(GLuint id, GLenum target);

        ~GLTexture();

        /**
         * @brief Get the texture target type
         * @return the texture target
         */
        [[nodiscard]] GLenum GetTarget() const;

        /**
         * @brief Get the identifier for the texture
         * @return the id
         */
        [[nodiscard]] GLuint GetNativeIdentifier() const;

    private:

        /**
         * @brief Destroy the texture resources
         */
        void Cleanup();

        /**
         * @brief OpenGL texture identifier
         */
        GLuint id_;

        /**
         * @brief The OpenGL texture target
         */
        GLenum target_;

    }; // class GLTexture

} // namespace zero::render