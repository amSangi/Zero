#include "render/renderer/opengl/GLShaderManager.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

GLShaderManager::GLShaderManager()
: shader_map_()
, program_map_()
{
}

GLShaderManager::~GLShaderManager()
{
}

std::shared_ptr<IProgram> GLShaderManager::GenerateProgram(const Material& material)
{
    std::vector<std::shared_ptr<GLShader>> shaders;
    std::string program_hash;

    auto search = shader_map_.find(material.shaders_.vertex_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(material.shaders_.fragment_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(material.shaders_.geometry_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(material.shaders_.tessellation_ctrl_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(material.shaders_.tessellation_eval_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    search = shader_map_.find(material.shaders_.compute_shader_);
    if (search != shader_map_.end())
    {
        shaders.push_back(search->second);
        program_hash += search->first;
    }

    auto program_search = program_map_.find(program_hash);
    if (program_search != program_map_.end())
    {
        return std::static_pointer_cast<IProgram>(program_search->second);
    }

    std::shared_ptr<GLProgram> program = GLProgram::CreateGLProgram(shaders);
    program_map_.emplace(program_hash, program);
    return program;
}

std::shared_ptr<IProgram> GLShaderManager::GenerateProgram(const SkyDome& sky_dome)
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

std::shared_ptr<IProgram> GLShaderManager::GenerateProgram(const std::string& vertex_shader,
                                                           const std::string& fragment_shader)
{
    std::string program_hash = vertex_shader + fragment_shader;
    auto program_search = program_map_.find(program_hash);
    if (program_search != program_map_.end())
    {
        return program_search->second;
    }

    std::vector<std::shared_ptr<GLShader>> shaders;
    auto shader_search = shader_map_.find(vertex_shader);
    if (shader_search != shader_map_.end())
    {
        shaders.push_back(shader_search->second);
    }
    shader_search = shader_map_.find(fragment_shader);
    if (shader_search != shader_map_.end())
    {
        shaders.push_back(shader_search->second);
    }

    auto program = GLProgram::CreateGLProgram(shaders);
    program_map_.emplace(program_hash, program);
    return program;
}

bool GLShaderManager::InitializeShader(const ShaderStage& stage)
{
    if (shader_map_.find(stage.name_) != shader_map_.end())
    {
        return true;
    }

    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(stage.source_, stage.type_);

    if (!shader->Compile())
    {
        std::string compile_error_message;
        GLint message_length = 0;
        glGetShaderiv(shader->GetNativeIdentifier(), GL_INFO_LOG_LENGTH, &message_length);
        compile_error_message.resize(message_length);
        glGetShaderInfoLog(shader->GetNativeIdentifier(), message_length, &message_length, compile_error_message.data());
        LOG_ERROR("GLShaderManager", compile_error_message);
        return false;
    }

    shader_map_[stage.name_] = shader;
    return true;
}

void GLShaderManager::ClearShaders()
{
    shader_map_.clear();
}

void GLShaderManager::ClearPrograms()
{
    program_map_.clear();
}

} // namespace zero::render
