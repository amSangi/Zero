#pragma once

#include <memory>
#include "OpenGL.hpp"
#include "render/IRenderResource.hpp"

namespace zero::render {

    /**
     * @brief OpenGL graphics texture wrapper
     */
    class GLTexture : public IRenderResource {
    public:

        /**
         * @brief Constructor
         * @param id the texture object identifier
         * @param target the texture target
         */
        GLTexture(GLuint id, GLenum target);

        ~GLTexture() override;

        /**
         * @brief Bind the texture object to the given texture unit
         * @param texture_unit the texture unit to bind to
         */
        void Bind(GLenum texture_unit) const;

        /**
         * @brief Generate mipmaps for the texture
         * @param texture_unit the texture unit to use
         */
        void GenerateMipMap(GLenum texture_unit) const;

        /**
         * @brief Get the texture target type
         * @return the texture target
         */
        GLenum GetTarget() const;

    protected:
        /**
         * @brief Destroy the texture resources
         */
        void Cleanup() override;

    private:

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