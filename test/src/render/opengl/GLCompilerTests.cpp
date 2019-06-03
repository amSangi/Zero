#include "gtest/gtest.h"
#include "render/opengl/TestGLCompiler.hpp"

using namespace zero::render;

TEST_F(TestGLCompiler, InitializeShaders) {
    EXPECT_TRUE(compiler_->InitializeShader(vertex_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(fragment_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(geometry_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(tess_ctrl_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(tess_eval_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(compute_stage_));
}

TEST_F(TestGLCompiler, CreateProgram) {
    EXPECT_TRUE(compiler_->InitializeShader(vertex_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(fragment_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(geometry_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(tess_ctrl_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(tess_eval_stage_));
    EXPECT_TRUE(compiler_->InitializeShader(compute_stage_));

    Material material;
    material.shaders_.vertex_shader_ = "Vertex";
    material.shaders_.fragment_shader_ = "Fragment";
    material.shaders_.compute_shader_ = "Unknown Compute Shader";

    auto program = compiler_->CreateProgram(material);
    ASSERT_NE(nullptr, program);
    EXPECT_TRUE(program->IsLinked());
}

TEST_F(TestGLCompiler, CreateProgramNoShaders) {
    Material material;
    material.shaders_.vertex_shader_ = "Vertex";
    material.shaders_.fragment_shader_ = "Fragment";

    auto program = compiler_->CreateProgram(material);
    ASSERT_EQ(nullptr, program);
}