#include "render/opengl/TestGLShader.hpp"

using namespace zero::render;

TEST_F(TestGLShader, ValidShader)
{
    GLShader vertex_shader(TEST_VERTEX_SOURCE, IShader::Type::VERTEX_SHADER);
    GLShader fragment_shader(TEST_FRAGMENT_SOURCE, IShader::Type::FRAGMENT_SHADER);
    GLShader geometry_shader(TEST_GEOMETRY_SOURCE, IShader::Type::GEOMETRY_SHADER);
    GLShader tess_ctrl_shader(TEST_TESS_CTRL_SOURCE, IShader::Type::TESSELLATION_CTRL_SHADER);
    GLShader tess_eval_shader(TEST_TESS_EVAL_SOURCE, IShader::Type::TESSELLATION_EVAL_SHADER);
    GLShader compute_shader(TEST_COMPUTE_SOURCE, IShader::Type::COMPUTE_SHADER);

    ExpectValidShader(vertex_shader);
    ExpectValidShader(fragment_shader);
    ExpectValidShader(geometry_shader);
    ExpectValidShader(tess_ctrl_shader);
    ExpectValidShader(tess_eval_shader);
    ExpectValidShader(compute_shader);
}

TEST_F(TestGLShader, InvalidShader)
{
    GLShader invalid_vertex_shader(TEST_INVALID_SOURCE, IShader::Type::VERTEX_SHADER);
    GLShader invalid_fragment_shader(TEST_INVALID_SOURCE, IShader::Type::FRAGMENT_SHADER);
    GLShader invalid_geometry_shader(TEST_INVALID_SOURCE, IShader::Type::GEOMETRY_SHADER);
    GLShader invalid_tess_ctrl_shader(TEST_INVALID_SOURCE, IShader::Type::TESSELLATION_CTRL_SHADER);
    GLShader invalid_tess_eval_shader(TEST_INVALID_SOURCE, IShader::Type::TESSELLATION_EVAL_SHADER);
    GLShader invalid_compute_shader(TEST_INVALID_SOURCE, IShader::Type::COMPUTE_SHADER);

    ExpectInvalidShader(invalid_vertex_shader);
    ExpectInvalidShader(invalid_fragment_shader);
    ExpectInvalidShader(invalid_geometry_shader);
    ExpectInvalidShader(invalid_tess_ctrl_shader);
    ExpectInvalidShader(invalid_tess_eval_shader);
    ExpectInvalidShader(invalid_compute_shader);
}

TEST_F(TestGLShader, Cleanup)
{
    GLuint value = 0;
    {
        GLShader vertex_shader(TEST_VERTEX_SOURCE, IShader::Type::VERTEX_SHADER);
        value = vertex_shader.GetNativeIdentifier();
        EXPECT_TRUE(IsShader(value));
    }
    EXPECT_FALSE(IsShader(value));
}

TEST_F(TestGLShader, VertexGeometrySourceMismatch)
{
    GLShader shader(TEST_GEOMETRY_SOURCE, IShader::Type::VERTEX_SHADER);
    ExpectInvalidShader(shader);
}