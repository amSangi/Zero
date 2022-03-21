#include <GL/glew.h>
#include <chrono>
#include <memory>
#include <thread>
#include "render/Image.hpp"
#include "render/renderer/opengl/GLShader.hpp"
#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLSampler.hpp"
#include "render/renderer/opengl/GLTextureManager.hpp"
#include "render/renderer/opengl/TestGLTexture.hpp"

using namespace zero::render;

static zero::uint32 kMicrosecondSleepTime = 150;

TEST_F(TestGLTexture, LoadImage)
{
    Image image("../resources/textures/test.png");

    // Load
    EXPECT_TRUE(image.Load());
    EXPECT_TRUE(image.IsLoaded());
    EXPECT_TRUE(image.Load());

    // Test loaded values
    EXPECT_NE(image.GetWidth(), 0);
    EXPECT_NE(image.GetHeight(), 0);
    EXPECT_NE(image.GetPitch(), 0);
    EXPECT_NE(image.GetPitch(), 0);
    EXPECT_NE(image.BitsPerPixel(), 0);
    EXPECT_NE(image.BytesPerPixel(), 0);
    EXPECT_NE(image.GetPixelFormat(), Image::PixelFormat::UNDEFINED);

    // Release
    EXPECT_TRUE(image.Release());

    // Test released values
    EXPECT_EQ(image.GetWidth(), 0);
    EXPECT_EQ(image.GetHeight(), 0);
    EXPECT_EQ(image.GetPitch(), 0);
    EXPECT_EQ(image.GetPitch(), 0);
    EXPECT_EQ(image.BitsPerPixel(), 0);
    EXPECT_EQ(image.BytesPerPixel(), 0);
    EXPECT_EQ(image.GetPixelFormat(), Image::PixelFormat::UNDEFINED);
}

TEST_F(TestGLTexture, LoadInvalidImage)
{
    Image image("");
    EXPECT_FALSE(image.Load());
    EXPECT_FALSE(image.IsLoaded());
    EXPECT_EQ(image.GetData(), nullptr);
}

TEST_F(TestGLTexture, CreateInvalidTexture)
{
    GLTextureManager manager;
    EXPECT_FALSE(manager.LoadTexture("", ""));
}

TEST_F(TestGLTexture, CreateTextureLargeIndex)
{
    GLTextureManager manager;
    EXPECT_TRUE(manager.LoadTexture(kTestImageName, kTestImageFile));
}

TEST_F(TestGLTexture, SetSamplerLargeIndex)
{
    GLTextureManager manager;
    auto sampler = std::make_shared<GLSampler>();
    sampler->Initialize();
    sampler->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    sampler->SetMagnificationFilter(ISampler::Filter::LINEAR);
    sampler->SetWrappingS(ISampler::Wrapping::REPEAT);
    sampler->SetWrappingT(ISampler::Wrapping::REPEAT);
    EXPECT_EQ(glGetError(), 0);
}

TEST_F(TestGLTexture, SetValidSampler)
{
    GLTextureManager manager;
    auto sampler = std::make_shared<GLSampler>();
    sampler->Initialize();
    sampler->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    sampler->SetMagnificationFilter(ISampler::Filter::LINEAR);
    sampler->SetWrappingS(ISampler::Wrapping::REPEAT);
    sampler->SetWrappingT(ISampler::Wrapping::REPEAT);
    EXPECT_EQ(glGetError(), 0);
}