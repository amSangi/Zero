#pragma once

#include "render/ShaderStage.hpp"

namespace zero::render {

    class GLDefaultShader {
    public:
        GLDefaultShader() = delete;
        ~GLDefaultShader() = delete;
        static ShaderStage kVertexShader;
        static ShaderStage kFragmentShader;
    }; // class GLDefaultShader

} // namespace zero::render
