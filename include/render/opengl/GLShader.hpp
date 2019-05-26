#pragma once

#include <string>
#include <SDL_opengl.h>
#include "../IShader.hpp"

namespace zero::render {

    class GLShader : public IShader {
    public:

        explicit GLShader(const std::string& source);

        ~GLShader();

        bool Compile() override;

        bool IsCompiled() const override;

        IShader::Type GetType() const override;

        GLuint GetNativeIdentifier();

    protected:
        void Cleanup() override;

    private:
        GLuint identifier_;

    }; // class GLShader

} // namespace zero::render