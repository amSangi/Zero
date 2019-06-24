#include "gtest/gtest.h"
#include "render/Image.hpp"
#include "render/opengl/TestGLTexture.hpp"

using namespace zero::render;

TEST_F(TestGLTexture, LoadImage) {
    Image image("../resources/textures/test.png");
    EXPECT_TRUE(image.Load());
    EXPECT_TRUE(image.IsLoaded());
    EXPECT_TRUE(image.Load());
    EXPECT_NE(image.GetWidth(), 0);
    EXPECT_NE(image.GetHeight(), 0);
    EXPECT_NE(image.GetPitch(), 0);
    EXPECT_NE(image.GetPitch(), 0);
}