#pragma once

#include <GL/glew.h>
#include <memory>
#include "TestWindow.hpp"
#include "render/opengl/GLShader.hpp"


class TestGLShader : public TestWindow {
protected:

    void SetUp() override {
        TestWindow::SetUp();
    }

    static bool IsShader(GLuint id) {
        return (glIsShader(id) == GL_TRUE);
    }

    static void ExpectValidShader(zero::render::GLShader& shader) {
        EXPECT_TRUE(shader.GetNativeIdentifier() != 0);
        EXPECT_TRUE(IsShader(shader.GetNativeIdentifier()));
        EXPECT_TRUE(shader.Compile());
    }

    static void ExpectInvalidShader(zero::render::GLShader& shader) {
        EXPECT_TRUE(shader.GetNativeIdentifier() != 0);
        EXPECT_TRUE(IsShader(shader.GetNativeIdentifier()));
        EXPECT_FALSE(shader.Compile());
    }

protected:
    const char* TEST_INVALID_SOURCE = "void main() {  ";

    const char* TEST_VERTEX_SOURCE = "#version 460\n"
                                     "layout (location = 0) in vec2 in_position;\n"
                                     "layout (location = 1) in vec3 in_colour;\n"
                                     "layout (location = 2) in vec2 in_texture_coordinate;\n"
                                     "layout (location = 0) out vec3 colour;\n"
                                     "layout (location = 1) out vec2 texture_coordinate;\n"
                                     "void main()\n"
                                     "{\n"
                                     "    colour = in_colour;\n"
                                     "    texture_coordinate = in_texture_coordinate;\n"
                                     "    gl_Position = vec4(in_position, 1.0, 1.0);\n"
                                     "}\n";

    const char* TEST_FRAGMENT_SOURCE = "#version 460\n"
                                       "uniform sampler2D tex_sampler;\n"
                                       "layout (location = 0) in vec2 colour;\n"
                                       "layout (location = 1) in vec2 texture_coordinate;\n"
                                       "layout(location = 0) out vec4 out_colour;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    out_colour = texture2D(tex_sampler, texture_coordinate);\n"
                                       "}\n";

    const char* TEST_GEOMETRY_SOURCE = "void main()"
                                          "{"
                                          "gl_Position = gl_in[0].gl_Position;"
                                          "EmitVertex();"
                                          "EndPrimitive();"
                                          "}";
    const char* TEST_TESS_CTRL_SOURCE = "void main() { }";
    const char* TEST_TESS_EVAL_SOURCE = "void main() { gl_Position = vec4(1,1,1,0); }";
    const char* TEST_COMPUTE_SOURCE = "void main() { }";

}; // class TestGLShader