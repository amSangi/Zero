#pragma once

#include "TestGLShader.hpp"
#include "render/renderer/ShaderStage.hpp"
#include "render/renderer/opengl/GLShaderManager.hpp"

class TestGLCompiler : public TestGLShader
{
protected:

    void SetUp() override
    {
        using namespace zero::render;
        TestGLShader::SetUp();
        shader_manager_ = std::make_shared<GLShaderManager>();
        vertex_stage_ = ShaderStage{IShader::Type::VERTEX_SHADER, "Vertex", TEST_VERTEX_SOURCE};
        fragment_stage_ = ShaderStage{IShader::Type::FRAGMENT_SHADER, "Fragment", TEST_FRAGMENT_SOURCE};
        geometry_stage_ = ShaderStage{IShader::Type::GEOMETRY_SHADER, "Geometry", TEST_GEOMETRY_SOURCE};
        tess_ctrl_stage_ = ShaderStage{IShader::Type::TESSELLATION_CTRL_SHADER, "Tess Ctrl", TEST_TESS_CTRL_SOURCE};
        tess_eval_stage_ = ShaderStage{IShader::Type::TESSELLATION_EVAL_SHADER, "Tess Eval", TEST_TESS_EVAL_SOURCE};
        compute_stage_ = ShaderStage{IShader::Type::COMPUTE_SHADER, "Compute", TEST_COMPUTE_SOURCE};
    }

protected:
    std::shared_ptr<zero::render::GLShaderManager> shader_manager_;
    zero::render::ShaderStage vertex_stage_;
    zero::render::ShaderStage fragment_stage_;
    zero::render::ShaderStage geometry_stage_;
    zero::render::ShaderStage tess_ctrl_stage_;
    zero::render::ShaderStage tess_eval_stage_;
    zero::render::ShaderStage compute_stage_;

}; // class TestGLCompiler