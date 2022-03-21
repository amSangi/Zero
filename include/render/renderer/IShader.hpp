#pragma once

#include "core/NonCopyable.hpp"

namespace zero::render
{

    /**
     * @brief A graphics shader program
     */
    class IShader : public NonCopyable
    {
    public:

        /**
         * @brief The Shader type
         */
        enum class Type
        {
            VERTEX_SHADER,             ///< Vertex Shader
            FRAGMENT_SHADER,           ///< Fragment/Pixel Shader
            GEOMETRY_SHADER,           ///< Geometry Shader
            TESSELLATION_CTRL_SHADER,  ///< Tessellation Control Shader
            TESSELLATION_EVAL_SHADER,  ///< Tessellation Evaluation Shader
            COMPUTE_SHADER             ///< Compute Shader
        }; // enum class Type

        virtual ~IShader() = default;

        /**
         * @brief The Shader type
         * @return the Type
         */
        [[nodiscard]] virtual Type GetType() const = 0;

    }; // class IShader

} // namespace zero::render