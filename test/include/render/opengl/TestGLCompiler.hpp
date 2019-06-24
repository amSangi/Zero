#pragma once

#include "TestGLShader.hpp"
#include "render/opengl/GLCompiler.hpp"

class TestGLCompiler : public TestGLShader {
protected:

    void SetUp() override {
        using namespace zero::render;
        TestGLShader::SetUp();
        compiler_ = std::make_shared<GLCompiler>();
        vertex_stage_ = ShaderStage{IShader::Type::TYPE_VERTEX_SHADER, "Vertex", TEST_VERTEX_SOURCE};
        fragment_stage_ = ShaderStage{IShader::Type::TYPE_FRAGMENT_SHADER, "Fragment", TEST_FRAGMENT_SOURCE};
        geometry_stage_ = ShaderStage{IShader::Type::TYPE_GEOMETRY_SHADER, "Geometry", TEST_GEOMETRY_SOURCE};
        tess_ctrl_stage_ = ShaderStage{IShader::Type::TYPE_TESSELLATION_CTRL_SHADER, "Tess Ctrl", TEST_TESS_CTRL_SOURCE};
        tess_eval_stage_ = ShaderStage{IShader::Type::TYPE_TESSELLATION_EVAL_SHADER, "Tess Eval", TEST_TESS_EVAL_SOURCE};
        compute_stage_ = ShaderStage{IShader::Type::TYPE_COMPUTE_SHADER, "Compute", TEST_COMPUTE_SOURCE};
    }

protected:
    std::shared_ptr<zero::render::GLCompiler> compiler_;
    zero::render::ShaderStage vertex_stage_;
    zero::render::ShaderStage fragment_stage_;
    zero::render::ShaderStage geometry_stage_;
    zero::render::ShaderStage tess_ctrl_stage_;
    zero::render::ShaderStage tess_eval_stage_;
    zero::render::ShaderStage compute_stage_;

}; // class TestGLCompiler