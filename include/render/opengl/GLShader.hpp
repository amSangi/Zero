#pragma once

#include <string>
#include "OpenGL.hpp"
#include "render/IShader.hpp"

namespace zero::render {

    /**
     * @brief OpenGL graphics shader wrapper
     */
    class GLShader : public IShader {
    public:

        /**
         * @brief Constructor
         * @param source the source code for the shader
         * @param type the shader type
         */
        GLShader(const std::string& source, IShader::Type type);

        ~GLShader() override;

        /**
         * @see IShader::Compile
         */
        bool Compile() override;

        /**
         * @see IShader::IsCompiled
         */
        bool IsCompiled() const override;

        /**
         * @see IShader::GetType
         */
        IShader::Type GetType() const override;

        /**
         * @brief Get the native OpenGL identifier
         * @return the identifier
         */
        GLuint GetNativeIdentifier() const;

    protected:
        /**
         * @brief Destroy the shader resources
         */
        void Cleanup() override;

    private:

        /**
         * @brief OpenGL shader identifier
         */
        GLuint id_;

        /**
         * @brief The shader type
         */
        const IShader::Type type_;

    }; // class GLShader

} // namespace zero::render