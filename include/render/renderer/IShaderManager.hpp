#pragma once

#include <memory>
#include <string>
#include "component/Material.hpp"
#include "component/SkyDome.hpp"
#include "core/NonCopyable.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/IShader.hpp"
#include "render/renderer/ShaderStage.hpp"

namespace zero::render
{

    class IShaderManager : public NonCopyable
    {
    public:
        virtual ~IShaderManager() = default;
        virtual std::shared_ptr<IProgram> GenerateProgram(const Material& material) = 0;
        virtual std::shared_ptr<IProgram> GenerateProgram(const SkyDome& sky_dome) = 0;
        virtual std::shared_ptr<IProgram> GenerateProgram(const std::string& vertex_shader, const std::string& fragment_shader) = 0;
        virtual bool InitializeShader(const ShaderStage& stage) = 0;
        virtual void ClearShaders() = 0;
        virtual void ClearPrograms() = 0;

    }; // class IShaderManager

} // namespace zero::render