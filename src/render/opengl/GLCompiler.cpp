#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLShader.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/Components.hpp"

using namespace zero::render;

GLCompiler::GLCompiler()
: shader_map_()
{}

std::shared_ptr<IProgram> GLCompiler::CreateProgram(const Material& material) {
    std::vector<std::shared_ptr<GLShader>> shaders;

    std::string shader_names;

    // Vertex Shader
    auto search = shader_map_.find(material.shaders_.vertex_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }
    else {
        // Use the default vertex shader if the shader cannot be found
        search = shader_map_.find(GLDefaultShader::kVertexShader.name_);
        if (search != shader_map_.end()) {
            shaders.push_back(search->second);
            shader_names += search->first;
        }
    }

    // Fragment Shader
    search = shader_map_.find(material.shaders_.fragment_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }
    else {
        // Use the default fragment shader if the shader cannot be found
        search = shader_map_.find(GLDefaultShader::kFragmentShader.name_);
        if (search != shader_map_.end()) {
            shaders.push_back(search->second);
            shader_names += search->first;
        }
    }

    // Geometry Shader
    search = shader_map_.find(material.shaders_.geometry_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }

    // Tessellation Control Shader
    search = shader_map_.find(material.shaders_.tessellation_ctrl_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }

    // Tessellation Evaluation Shader
    search = shader_map_.find(material.shaders_.tessellation_eval_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }

    // Compute Shader
    search = shader_map_.find(material.shaders_.compute_shader_);
    if (search != shader_map_.end()) {
        shaders.push_back(search->second);
        shader_names += search->first;
    }

    // Return cached program
    auto program_search = program_map_.find(shader_names);
    if (program_search != program_map_.end()) {
        return program_search->second;
    }

    // Cache program
    auto program = GLProgram::CreateGLProgram(shaders);
    program_map_[shader_names] = program;
    return program;
}

bool GLCompiler::InitializeShader(const ShaderStage& stage) {
    if (shader_map_.find(stage.name_) != shader_map_.end()) return true;

    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(stage.source_, stage.type_);

    if (!shader->Compile()) {
        return false;
    }

    shader_map_[stage.name_] = shader;
    return true;
}

void GLCompiler::ClearShaders() {
    shader_map_.clear();
}

void GLCompiler::ClearPrograms() {
    program_map_.clear();
}