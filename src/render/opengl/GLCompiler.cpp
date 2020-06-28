#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLShader.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/ShaderStage.hpp"
#include "component/Material.hpp"
#include "component/SkyDome.hpp"

namespace zero::render
{

GLCompiler::GLCompiler()
: shader_map_()
{
}

std::shared_ptr<GLProgram> GLCompiler::CreateProgram(const Material& material)
{
    std::vector<std::shared_ptr<GLShader>> shaders;
    std::string program_hash;

    // Vertex Shader
    auto search = shader_map_.find(material.shaders_.vertex_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Fragment Shader
    search = shader_map_.find(material.shaders_.fragment_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Geometry Shader
    search = shader_map_.find(material.shaders_.geometry_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Tessellation Control Shader
    search = shader_map_.find(material.shaders_.tessellation_ctrl_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Tessellation Evaluation Shader
    search = shader_map_.find(material.shaders_.tessellation_eval_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Compute Shader
    search = shader_map_.find(material.shaders_.compute_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Check program cache
    auto program_search = program_map_.find(program_hash);
    if (program_search != program_map_.end())
    {
        return program_search->second;
    }

    // Cache a new program
    auto program = GLProgram::CreateGLProgram(shaders);
    program_map_.emplace(program_hash, program);
    return program;
}

std::shared_ptr<GLProgram> GLCompiler::CreateProgram(const SkyDome& sky_dome)
{
    std::vector<std::shared_ptr<GLShader>> shaders;
    std::string program_hash;

    auto search = shader_map_.find(sky_dome.vertex_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(sky_dome.fragment_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    // Check program cache
    auto program_search = program_map_.find(program_hash);
    if (program_search != program_map_.end())
    {
        return program_search->second;
    }

    // Cache a new program
    auto program = GLProgram::CreateGLProgram(shaders);
    program_map_.emplace(program_hash, program);
    return program;
}


bool GLCompiler::InitializeShader(const ShaderStage& stage)
{
    if (shader_map_.find(stage.name_) != shader_map_.end())
    {
        return true;
    }

    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(stage.source_, stage.type_);

    if (!shader->Compile())
    {
        return false;
    }

    shader_map_[stage.name_] = shader;
    return true;
}

void GLCompiler::ClearShaders()
{
    shader_map_.clear();
}

void GLCompiler::ClearPrograms()
{
    program_map_.clear();
}

} // namespace zero::render