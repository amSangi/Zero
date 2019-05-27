#pragma once

#include "IRenderResource.hpp"

namespace zero::render {

    /**
     * @brief A graphics shader program
     */
    class IShader : public IRenderResource {
    public:

        /**
         * @brief The Shader type
         */
        enum class Type {
            TYPE_UNKNOWN,                   ///< Unknown Shader
            TYPE_VERTEX_SHADER,             ///< Vertex Shader
            TYPE_FRAGMENT_SHADER,           ///< Fragment/Pixel Shader
            TYPE_GEOMETRY_SHADER,           ///< Geometry Shader
            TYPE_TESSELLATION_CTRL_SHADER,  ///< Tessellation Control Shader
            TYPE_TESSELLATION_EVAL_SHADER,  ///< Tessellation Evaluation Shader
            TYPE_COMPUTE_SHADER             ///< Compute Shader
        }; // enum class Type

        /**
         * @brief Default Constructor
         */
        IShader() = default;

        /**
         * @brief Virtual Destructor
         */
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
        virtual bool IsCompiled() const = 0;

        /**
         * @brief The Shader type
         * @return the Type
         */
        virtual Type GetType() const = 0;

    }; // class IShader

} // namespace zero::render