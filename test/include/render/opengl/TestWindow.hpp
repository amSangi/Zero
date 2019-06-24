#pragma once

#include <memory>
#include "gtest/gtest.h"
#include "render/Window.hpp"

class TestWindow : public ::testing::Test {
protected:

    void SetUp() override {
        zero::render::WindowConfig window_config{zero::render::WindowFlags::WINDOW_HIDE,
                                                 0,
                                                 0,
                                                 zero::render::WindowMode::WINDOW_WINDOWED,
                                                 zero::render::GraphicsAPI::GRAPHICS_OPENGL,
                                                 zero::render::RefreshRate::REFRESH_IMMEDIATE,
                                                 "Test Window"};

        test_window_ = std::make_shared<zero::render::Window>(window_config);
        test_window_->Initialize();
    }

private:
    std::shared_ptr<zero::render::Window> test_window_;

}; // class TestWindow