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
        SetUp(window_config);
    }

    virtual void SetUp(zero::render::WindowConfig config) {
        test_window_ = std::make_shared<zero::render::Window>(config);
        test_window_->Initialize();
    }

protected:
    std::shared_ptr<zero::render::Window> test_window_;

}; // class TestWindow