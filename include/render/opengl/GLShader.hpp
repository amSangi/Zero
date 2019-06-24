#pragma once

#include <string>
#include <SDL_opengl.h>
#include "render/IShader.hpp"

namespace zero::render {

    class GLShader : public IShader {
    public:

        GLShader(const std::string& source, IShader::Type type);

        ~GLShader() override;

        bool Compile() override;

        bool IsCompiled() const override;

        IShader::Type GetType() const override;

        GLuint GetNativeIdentifier();

    protected:
        void Cleanup() override;

    private:
        GLuint id_;
        const IShader::Type type_;

    }; // class GLShader

} // namespace zero::render