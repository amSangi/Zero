#pragma once

#include "TestGLShader.hpp"
#include "render/renderer/opengl/GLTexture.hpp"

class TestGLTexture : public TestGLShader
{
protected:

    void SetUp() override
    {
        zero::WindowConfig window_config{zero::WindowFlags::NO_FLAGS,
                                         800,
                                         600,
                                         zero::WindowMode::WINDOWED,
                                         zero::GraphicsAPI::OPENGL,
                                         zero::RefreshRate::IMMEDIATE,
                                         "Test Window"};
        TestWindow::SetUp(window_config);
    }

protected:
    std::string kTestImageFile = "../resources/textures/test.png";
    std::string kTestImageName = "test.png";

}; // class TestGLTexture