#include <GL/glew.h>
#include <chrono>
#include <memory>
#include <thread>
#include "render/renderer/Image.hpp"
#include "render/opengl/GLShader.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/TestGLTexture.hpp"

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
    EXPECT_FALSE(manager.InitializeImage("", ""));
}

TEST_F(TestGLTexture, CreateTextureLargeIndex)
{
    GLTextureManager manager;
    EXPECT_TRUE(manager.InitializeImage(kTestImageName, kTestImageFile));
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
    manager.SetSampler(sampler, manager.GetTextureUnitCount());
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
    manager.SetSampler(sampler, 0);
    EXPECT_EQ(glGetError(), 0);
}

TEST_F(TestGLTexture, TextureWithShader)
{
    // Create Shaders
    std::vector<std::shared_ptr<GLShader>> shaders;
    auto vertex_shader = std::make_shared<GLShader>(TEST_VERTEX_SOURCE, IShader::Type::VERTEX_SHADER);
    EXPECT_TRUE(vertex_shader->Compile());
    auto fragment_shader = std::make_shared<GLShader>(TEST_FRAGMENT_SOURCE, IShader::Type::FRAGMENT_SHADER);
    EXPECT_TRUE(fragment_shader->Compile());

    shaders.push_back(vertex_shader);
    shaders.push_back(fragment_shader);

    // Create Graphics Program
    auto program = GLProgram::CreateGLProgram(shaders);
    ASSERT_NE(program, nullptr);

    // Test vertices
    GLfloat vertices[] =
    {
            //  Position      Color             Texcoords
           -0.5F,  0.5F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, // Top-left
            0.5F,  0.5F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F, // Top-right
            0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, // Bottom-right
           -0.5F, -0.5F, 1.0F, 1.0F, 1.0F, 0.0F, 1.0F  // Bottom-left
    };
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    GLuint elements[] =
    {
            0, 1, 2,
            2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    ASSERT_TRUE(program->IsLinked());
    program->Use();

    // Set layout
    GLint pos_attrib = program->GetAttribLocation("in_position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), nullptr);

    GLint col_attrib = program->GetAttribLocation("in_colour");
    glEnableVertexAttribArray(col_attrib);
    glVertexAttribPointer(col_attrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));


    GLint tex_attrib = program->GetAttribLocation("in_texture_coordinate");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

    // Set texture sampler
    GLTextureManager manager;
    zero::uint8 index = 0;
    auto sampler = std::make_shared<GLSampler>();
    sampler->Initialize();
    sampler->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    sampler->SetMagnificationFilter(ISampler::Filter::LINEAR);
    sampler->SetWrappingS(ISampler::Wrapping::REPEAT);
    sampler->SetWrappingT(ISampler::Wrapping::REPEAT);
    manager.SetSampler(sampler, index);

    // Create texture
    EXPECT_TRUE(manager.InitializeImage(kTestImageName, kTestImageFile));
    EXPECT_TRUE(manager.InitializeGLTextures());
    auto texture = manager.GetGLTexture(kTestImageName);
    ASSERT_NE(texture, nullptr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texture->GetTarget(), texture->GetNativeIdentifier());

    program->SetUniform("tex_sampler", index);
    program->FlushUniforms();

    EXPECT_EQ(glGetError(), 0);

    // Clear the screen to black
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    test_window_->SwapBuffers();

    std::this_thread::sleep_for(std::chrono::milliseconds(kMicrosecondSleepTime));
}