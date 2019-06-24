#pragma once

#include "TestWindow.hpp"
#include "render/opengl/GLTexture.hpp"

class TestGLTexture : public TestWindow {
protected:

    void SetUp() override {
        TestWindow::SetUp();
    }

private:
    const char* TEST_VERTEX_SOURCE = "in vec2 position;"
                                     "in vec2 in_texture_coordinate;"
                                     "out vec2 texture_coordinate;"
                                     "void main()"
                                     "{"
                                     "    texture_coordinate = in_texture_coordinate;"
                                     "    gl_Position = vec4(position, 0.0, 1.0);"
                                     "}";

    const char* TEST_FRAGMENT_SOURCE = "uniform sampler2D texture;"
                                       "in vec2 texture_coordinate;"
                                       "void main()"
                                       "{"
                                       "    gl_FragColor = texture(texture, texture_coordinate);"
                                       "}";

}; // class TestGLTexture