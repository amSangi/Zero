#pragma once

#include <string>
#include "component/Material.hpp"
#include "component/SkyDome.hpp"
#include "core/NonCopyable.hpp"
#include "render/IShader.hpp"
#include "render/ShaderStage.hpp"

namespace zero::render
{

    class IShaderManager : public NonCopyable
    {
    public:
        virtual ~IShaderManager() = default;
        virtual IProgram* GenerateProgram(const Material& material) = 0;
        virtual IProgram* GenerateProgram(const SkyDome& sky_dome) = 0;
        virtual IProgram* GenerateProgram(const std::string& vertex_shader, const std::string& fragment_shader) = 0;
        virtual bool InitializeShader(const ShaderStage& stage) = 0;
        virtual void ClearShaders() = 0;
        virtual void ClearPrograms() = 0;

    }; // class IShaderManager

} // namespace zero::render