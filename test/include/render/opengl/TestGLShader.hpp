#pragma once

#include "GL/glew.h"
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
    const char* TEST_VERTEX_SOURCE = "void main() { gl_Position = gl_Vertex; }";
    const char* TEST_FRAGMENT_SOURCE = "void main() { gl_FragColor = vec4(1,1,0,1); }";
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