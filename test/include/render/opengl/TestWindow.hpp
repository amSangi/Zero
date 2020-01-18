#pragma once

#include <memory>
#include <gtest/gtest.h>
#include "render/Window.hpp"
#include "core/WindowConfig.hpp"

class TestWindow : public ::testing::Test {
protected:

    void SetUp() override {
        zero::WindowConfig window_config{zero::WindowFlags::HIDE,
                                         0,
                                         0,
                                         zero::WindowMode::WINDOWED,
                                         zero::GraphicsAPI::OPENGL,
                                         zero::RefreshRate::IMMEDIATE,
                                         "Test Window"};
        SetUp(window_config);
    }

    virtual void SetUp(zero::WindowConfig config) {
        test_window_ = std::make_shared<zero::render::Window>(config);
        test_window_->Initialize();
    }

protected:
    std::shared_ptr<zero::render::Window> test_window_;

}; // class TestWindow