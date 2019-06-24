#include "gtest/gtest.h"
#include "render/opengl/TestGLProgram.hpp"

using namespace zero::render;

TEST_F(TestGLProgram, LinkEmptyProgram) {
    std::vector<std::shared_ptr<GLShader>> shaders;
    auto program = GLProgram::CreateGLProgram(shaders);
    ASSERT_EQ(program, nullptr);
}

TEST_F(TestGLProgram, LinkSingleVertexFragmentShaderProgram) {
    // Vertex
    std::vector<std::shared_ptr<GLShader>> shaders;
    shaders.push_back(vertex_shader_);
    auto program = GLProgram::CreateGLProgram(shaders);
    ASSERT_NE(program, nullptr);
    EXPECT_TRUE(program->IsLinked());

    // Fragment
    shaders.clear();
    shaders.push_back(fragment_shader_);
    program = GLProgram::CreateGLProgram(shaders);
    ASSERT_NE(program, nullptr);
    EXPECT_TRUE(program->IsLinked());
}

TEST_F(TestGLProgram, LinkVertexFragmentProgram) {
    std::vector<std::shared_ptr<GLShader>> shaders;
    shaders.push_back(vertex_shader_);
    shaders.push_back(fragment_shader_);

    auto program = GLProgram::CreateGLProgram(shaders);
    ASSERT_NE(program, nullptr);
    EXPECT_TRUE(program->IsLinked());
}

TEST_F(TestGLProgram, Cleanup) {
    GLuint id;
    {
        std::vector<std::shared_ptr<GLShader>> shaders;
        shaders.push_back(vertex_shader_);
        auto program = GLProgram::CreateGLProgram(shaders);
        id = program->GetNativeIdentifier();
        EXPECT_TRUE(IsProgram(id));
    }
    EXPECT_FALSE(IsProgram(id));
}
