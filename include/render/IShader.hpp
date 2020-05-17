#pragma once

#include "IRenderResource.hpp"

namespace zero::render {

    /**
     * @brief A graphics shader program
     */
    class IShader : public IRenderResource
    {
    public:

        /**
         * @brief The Shader type
         */
        enum class Type
        {
            UNKNOWN,                   ///< Unknown Shader
            VERTEX_SHADER,             ///< Vertex Shader
            FRAGMENT_SHADER,           ///< Fragment/Pixel Shader
            GEOMETRY_SHADER,           ///< Geometry Shader
            TESSELLATION_CTRL_SHADER,  ///< Tessellation Control Shader
            TESSELLATION_EVAL_SHADER,  ///< Tessellation Evaluation Shader
            COMPUTE_SHADER             ///< Compute Shader
        }; // enum class Type

        ~IShader() override = default;

        /**
         * @brief Compile the Shader program
         * @return True if the compilation was successful. Otherwise false.
         */
        virtual bool Compile() = 0;

        /**
         * @brief Check the compilation status of the shader
         * @return True if the shader is compiled. Otherwise false.
         */
        [[nodiscard]] virtual bool IsCompiled() const = 0;

        /**
         * @brief The Shader type
         * @return the Type
         */
        [[nodiscard]] virtual Type GetType() const = 0;

    }; // class IShader

} // namespace zero::render