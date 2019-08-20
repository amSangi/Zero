#pragma once

#include "TestGLShader.hpp"
#include "render/opengl/GLProgram.hpp"

class TestGLProgram : public TestGLShader {
protected:
    void SetUp() override {
        using namespace zero::render;
        TestGLShader::SetUp();
        vertex_shader_ = std::make_shared<zero::render::GLShader>(TEST_VERTEX_SOURCE, IShader::Type::VERTEX_SHADER);
        fragment_shader_ = std::make_shared<zero::render::GLShader>(TEST_FRAGMENT_SOURCE, IShader::Type::FRAGMENT_SHADER);
        geometry_shader_ = std::make_shared<zero::render::GLShader>(TEST_GEOMETRY_SOURCE, IShader::Type::GEOMETRY_SHADER);
        tess_ctrl_shader_ = std::make_shared<zero::render::GLShader>(TEST_TESS_CTRL_SOURCE, IShader::Type::TESSELLATION_CTRL_SHADER);
        tess_eval_shader_ = std::make_shared<zero::render::GLShader>(TEST_TESS_EVAL_SOURCE, IShader::Type::TESSELLATION_EVAL_SHADER);
        compute_shader_ = std::make_shared<zero::render::GLShader>(TEST_COMPUTE_SOURCE, IShader::Type::COMPUTE_SHADER);

        ASSERT_TRUE(vertex_shader_->Compile());
        ASSERT_TRUE(fragment_shader_->Compile());
        ASSERT_TRUE(geometry_shader_->Compile());
        ASSERT_TRUE(tess_ctrl_shader_->Compile());
        ASSERT_TRUE(tess_eval_shader_->Compile());
        ASSERT_TRUE(compute_shader_->Compile());
    }

protected:
    std::shared_ptr<zero::render::GLShader> vertex_shader_;
    std::shared_ptr<zero::render::GLShader> fragment_shader_;
    std::shared_ptr<zero::render::GLShader> geometry_shader_;
    std::shared_ptr<zero::render::GLShader> tess_ctrl_shader_;
    std::shared_ptr<zero::render::GLShader> tess_eval_shader_;
    std::shared_ptr<zero::render::GLShader> compute_shader_;

}; // class TestGLProgram