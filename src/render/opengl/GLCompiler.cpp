#include "render/opengl/GLShader.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/opengl/GLCompiler.hpp"

using namespace zero::render;

bool GLCompiler::InitializeShader(ICompiler::ShaderStage stage) {
    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(stage.source_, stage.type_);

    if (shader->Compile()) return false;

    shader_map_[stage.name_] = shader;
    return true;
}

std::shared_ptr<IProgram> GLCompiler::CreateProgram(Material material) {
    std::vector<std::shared_ptr<GLShader>> shaders;

    // Vertex Shader
    auto search = shader_map_.find(material.shaders_.vertex_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    // Fragment Shader
    search = shader_map_.find(material.shaders_.fragment_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    // Geometry Shader
    search = shader_map_.find(material.shaders_.geometry_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    // Tessellation Control Shader
    search = shader_map_.find(material.shaders_.tessellation_ctrl_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    // Tessellation Evaluation Shader
    search = shader_map_.find(material.shaders_.tessellation_eval_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    // Compute Shader
    search = shader_map_.find(material.shaders_.compute_shader_);
    if (search != shader_map_.end()) shaders.push_back(search->second);

    return GLProgram::CreateGLProgram(shaders);
}