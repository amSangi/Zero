#pragma once

#include <string>
#include "OpenGL.hpp"
#include "render/IShader.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL graphics shader wrapper
     */
    class GLShader final : public IShader
    {
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
        [[nodiscard]] bool IsCompiled() const override;

        /**
         * @see IShader::GetType
         */
        [[nodiscard]] IShader::Type GetType() const override;

        /**
         * @brief Get the native OpenGL identifier
         * @return the identifier
         */
        [[nodiscard]] GLuint GetNativeIdentifier() const;

    private:

        /**
         * @brief Destroy the shader resources
         */
        void Cleanup() const;

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