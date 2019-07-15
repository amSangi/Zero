#pragma once

#include <memory>
#include <SDL_opengl.h>
#include "render/Image.hpp"
#include "render/ITexture.hpp"

namespace zero::render {

    class GLTexture : public ITexture {
    public:

        GLTexture(GLuint id, GLenum target, GLenum texture_unit);

        ~GLTexture() override;

        void Use() const;

        void GenerateMipMap() const;

        void SetTextureUnit(GLenum texture_unit);

        GLuint GetNativeIdentifier() const;

        GLenum GetTarget() const;

        GLenum GetTextureUnit() const;


    protected:
        void Cleanup() override;

    private:
        GLuint id_;
        GLenum target_;
        GLenum texture_unit_;

    }; // class GLTexture

} // namespace zero::render