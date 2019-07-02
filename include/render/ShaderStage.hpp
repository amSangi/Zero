#pragma once

#include <string>
#include "IShader.hpp"

namespace zero::render {

    /**
     * @brief Container for a Graphics Shader
     */
    struct ShaderStage {

        /**
         * @brief The Shader Type
         */
        IShader::Type type_;

        /**
         * @brief The Shader filename
         */
        std::string filename_;

        /**
         * @brief The Shader source code
         */
        std::string source_;

    }; // struct ShaderStage

} // namespace zero::render