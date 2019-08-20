#pragma once

#include "TestGLShader.hpp"
#include "render/opengl/GLTexture.hpp"

class TestGLTexture : public TestGLShader {
protected:

    void SetUp() override {
        zero::render::WindowConfig window_config{zero::render::WindowFlags::NO_FLAGS,
                                                 800,
                                                 600,
                                                 zero::render::WindowMode::WINDOWED,
                                                 zero::render::GraphicsAPI::OPENGL,
                                                 zero::render::RefreshRate::IMMEDIATE,
                                                 "Test Window"};
        TestWindow::SetUp(window_config);
    }

protected:

    std::string TEST_IMAGE_FILE = "../resources/textures/test.png";

}; // class TestGLTexture