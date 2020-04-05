#pragma once

#include "render/ShaderStage.hpp"

namespace zero::render {

    /**
     * @brief Static container of default GLSL shader programs
     */
    class GLDefaultShader {
    public:
        GLDefaultShader() = delete;
        ~GLDefaultShader() = delete;
        /**
         * @brief A default GLSL vertex shader for primitives
         */
        static ShaderStage kVertexShader;
        /**
         * @brief A default GLSL fragment shader for primitives
         */
        static ShaderStage kFragmentShader;
    }; // class GLDefaultShader

} // namespace zero::render
