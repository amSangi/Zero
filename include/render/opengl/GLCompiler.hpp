#pragma once

#include <string>
#include <unordered_map>
#include "GLShader.hpp"
#include "../ICompiler.hpp"

namespace zero::render {

    class GLCompiler : public ICompiler {
    public:

        GLCompiler() = default;

        ~GLCompiler() override = default;

        bool InitializeShader(const ShaderStage& stage) override;

        std::shared_ptr<IProgram> CreateProgram(const Material& material) override;

    private:
        std::unordered_map<std::string, std::shared_ptr<GLShader>> shader_map_;

    }; // class GLCompiler

} // namespace zero::render