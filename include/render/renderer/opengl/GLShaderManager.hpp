#pragma once

#include <unordered_map>
#include "render/renderer/IShaderManager.hpp"
#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLShader.hpp"

namespace zero::render
{

    class GLShaderManager final : public IShaderManager
    {
    public:
        GLShaderManager();
        ~GLShaderManager() override;
        std::shared_ptr<IProgram> GenerateProgram(const Material& material) override;
        std::shared_ptr<IProgram> GenerateProgram(const SkyDome& sky_dome) override;
        std::shared_ptr<IProgram> GenerateProgram(const std::string& vertex_shader, const std::string& fragment_shader) override;
        bool InitializeShader(const ShaderStage& stage) override;
        void ClearShaders() override;
        void ClearPrograms() override;

    private:

        /**
         * @brief Filename to GLShader map
         */
        std::unordered_map<std::string, std::shared_ptr<GLShader>> shader_map_;

        /**
         * @brief Concatenated shader name to graphics program map
         * Graphics program caching avoids having to link shaders that have already been linked again.
         */
        std::unordered_map<std::string, std::shared_ptr<GLProgram>> program_map_;

    }; // class GLShaderManager

} // namespace zero::render