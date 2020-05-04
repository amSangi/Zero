#pragma once

#include <string>
#include "OpenGL.hpp"
#include "render/IRenderResource.hpp"

namespace zero::render {

    /**
     * @brief OpenGL graphics texture wrapper
     */
    class GLTexture final : public IRenderResource {
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
        [[nodiscard]] GLenum GetTarget() const;

        /**
         * @brief Get the identifier for the texture
         * @return the id
         */
        [[nodiscard]] GLuint GetIdentifier() const;

        /**
         * @brief Get the name
         * @return the name. Empty string if one is not set.
         */
        [[nodiscard]] std::string GetName() const;

        /**
         * @brief Set the name of the texture (e.g. `diffuse_texture`)
         * @param name the name
         */
        void SetName(const std::string& name);

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

        /**
         * @brief The shader sampler name the texture is associated with
         */
        std::string name_;

    }; // class GLTexture

} // namespace zero::render