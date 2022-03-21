#pragma once

#include <string>
#include "render/renderer/IShader.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

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
         * @param shader_id the OpenGL Shader ID
         * @param type the shader type
         */
        GLShader(GLuint shader_id, IShader::Type type);

        ~GLShader() override = default;

        /**
         * @see IShader::GetType
         */
        [[nodiscard]] IShader::Type GetType() const override;

        /**
         * @brief Get the native OpenGL identifier
         * @return the identifier
         */
        [[nodiscard]] GLuint GetIdentifier() const;

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