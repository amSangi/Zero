#include "render/renderer/opengl/OpenGL.hpp"
#include "render/renderer/opengl/GLFrameBuffer.hpp"
#include "render/renderer/opengl/GLMesh.hpp"
#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLRenderHardware.hpp"
#include "render/renderer/opengl/GLSampler.hpp"
#include "render/renderer/opengl/GLShader.hpp"
#include "render/renderer/opengl/GLTexture.hpp"
#include "render/renderer/opengl/GLUniformBuffer.hpp"
#include "core/Logger.hpp"
#include <cassert>

namespace zero::render
{

void GLMessageCallback(GLenum /* source */,
                       GLenum type,
                       GLuint /* id */,
                       GLenum /* severity */,
                       GLsizei length,
                       const GLchar* message,
                       const void* /* user_param */)
{
    constexpr auto kOpenGLTitle = "GLMessageCallback";

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        LOG_ERROR(kOpenGLTitle, std::string(message, length));
    }
    else
    {
        LOG_DEBUG(kOpenGLTitle, std::string(message, length));
    }
}

const char* GLRenderHardware::kTitle = "GLRenderHardware";

GLRenderHardware::GLRenderHardware()
: diffuse_map_sampler_(nullptr)
, shadow_map_sampler_(nullptr)
, meshes_()
, shaders_()
, programs_()
, textures_()
, uniform_buffers_()
, bound_shader_program_(nullptr)
, available_texture_unit_index_(0)
, available_uniform_buffer_binding_point(0)
{
}

void GLRenderHardware::Initialize()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

    // Create diffuse and shadow texture samplers
    GLuint sampler_ids[] = {0, 0};
    glGenSamplers(2, sampler_ids);
    diffuse_map_sampler_ = std::make_shared<GLSampler>(sampler_ids[0]);
    diffuse_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    diffuse_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    diffuse_map_sampler_->SetWrappingS(ISampler::Wrapping::REPEAT);
    diffuse_map_sampler_->SetWrappingT(ISampler::Wrapping::REPEAT);

    shadow_map_sampler_ = std::make_shared<GLSampler>(sampler_ids[1]);
    shadow_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetWrappingS(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetWrappingT(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetBorderColour(math::Vec4f::One());
    glSamplerParameteri(shadow_map_sampler_->GetIdentifier(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glSamplerParameteri(shadow_map_sampler_->GetIdentifier(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    // Create shadow map textures and frame buffers
    for (uint32 i = 0; i < Constants::kShadowCascadeCount; ++i)
    {
        // Create a texture for the shadow map at the given cascade
        GLuint shadow_map_texture_id = 0;
        glGenTextures(1, &shadow_map_texture_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_map_texture_id);
        glTexImage2D(GL_TEXTURE_2D,                  // Target texture.
                     0,                              // Level of detail. Level n is the nth mipmap reduction image.
                     GL_DEPTH_COMPONENT,             // Number of colour components in the texture.
                     Constants::kShadowMapWidth,     // Texture image width.
                     Constants::kShadowMapHeight,    // Texture image height.
                     0,                              // Border. Must be 0.
                     GL_DEPTH_COMPONENT,             // Format of the pixel data.
                     GL_FLOAT,                       // Data type of the pixel data.
                     nullptr);                 // Pointer to the image data in memory.

        // Associate a separate frame buffer for each shadow map texture
        GLuint shadow_map_frame_buffer_id = 0;
        glGenFramebuffers(1, &shadow_map_frame_buffer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_frame_buffer_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_texture_id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        std::shared_ptr<GLTexture> texture = std::make_shared<GLTexture>(shadow_map_texture_id, GL_TEXTURE_2D);
        shadow_map_textures_.push_back(texture);

        std::shared_ptr<GLFrameBuffer> frame_buffer = std::make_shared<GLFrameBuffer>(shadow_map_frame_buffer_id);
        shadow_map_frame_buffers_.push_back(frame_buffer);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create 1x1 black texture for unused/empty/default textures
    GLuint empty_texture;
    glGenTextures(1, &empty_texture);
    glBindTexture(GL_TEXTURE_2D, empty_texture);
    GLubyte data[] = {0, 0, 0, 0};
    glTexImage2D(GL_TEXTURE_2D,     // Target texture)
                 0,                 // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGBA,           // Number of color components in the texture.
                 1,                 // Texture image width
                 1,                 // Texture image height
                 0,                 // Border. Must be 0.
                 GL_RGBA,           // Format of the pixel data
                 GL_UNSIGNED_BYTE,  // Data type of the pixel data
                 data);             // Pointer to the image data in memory
    textures_.push_back(empty_texture);
    glBindTexture(GL_TEXTURE_2D, 0);
    empty_texture_ = std::make_shared<GLTexture>(empty_texture, GL_TEXTURE_2D);

#if LOGGING_ENABLED
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLMessageCallback, nullptr);
#endif
}

void GLRenderHardware::Shutdown()
{
    GLuint diffuse_sampler_id = diffuse_map_sampler_->GetIdentifier();
    GLuint shadow_sampler_id = shadow_map_sampler_->GetIdentifier();
    glDeleteSamplers(1, &diffuse_sampler_id);
    glDeleteSamplers(1, &shadow_sampler_id);
    for (const std::shared_ptr<GLMesh>& gl_mesh : meshes_)
    {
        GLuint vao_id = gl_mesh->GetVAOIdentifier();
        GLuint vertex_buffer_id = gl_mesh->GetVertexBufferIdentifier();
        GLuint index_buffer_id = gl_mesh->GetIndexBufferIdentifier();
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vertex_buffer_id);
        glDeleteBuffers(1, &index_buffer_id);
    }
    for (GLuint program_id : programs_)
    {
        glDeleteProgram(program_id);
    }
    for (GLuint shader_id : shaders_)
    {
        glDeleteShader(shader_id);
    }
    glDeleteTextures(static_cast<GLint>(textures_.size()), textures_.data());
    glDeleteBuffers(static_cast<GLint>(uniform_buffers_.size()), uniform_buffers_.data());
}

void GLRenderHardware::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLint>(width), static_cast<GLint>(height));
}

void GLRenderHardware::SetFillMode(FillMode fill_mode)
{
    switch (fill_mode)
    {
        case FillMode::FILL_MODE_WIREFRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FillMode::FILL_MODE_SOLID:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void GLRenderHardware::SetCullMode(CullMode cull_mode)
{
    switch (cull_mode)
    {
        case CullMode::CULL_MODE_NONE:
            glDisable(GL_CULL_FACE);
            break;
        case CullMode::CULL_MODE_FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullMode::CULL_MODE_BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
    }
}

void GLRenderHardware::SetClearColor(const math::Vec4f& color)
{
    glClearColor(color.x_, color.y_, color.z_, color.w_);
}

void GLRenderHardware::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

std::shared_ptr<ISampler> GLRenderHardware::GetDiffuseMapSampler()
{
    return diffuse_map_sampler_;
}

std::shared_ptr<ISampler> GLRenderHardware::GetShadowMapSampler()
{
    return shadow_map_sampler_;
}

const std::vector<std::shared_ptr<ITexture>>& GLRenderHardware::GetShadowMapTextures()
{
    return shadow_map_textures_;
}

const std::vector<std::shared_ptr<IFrameBuffer>>& GLRenderHardware::GetShadowMapFrameBuffers()
{
    return shadow_map_frame_buffers_;
}

void GLRenderHardware::UpdateUniformData(std::shared_ptr<IUniformBuffer> uniform_buffer,
                                         const void *data,
                                         uint32 data_size,
                                         uint32 data_offset)
{
    if (!uniform_buffer)
    {
        return;
    }

    if (data_size + data_offset > uniform_buffer->GetSize())
    {
        LOG_ERROR(kTitle, "Data size is greater than total uniform buffer size");
        return;
    }

    std::shared_ptr<GLUniformBuffer> gl_uniform_buffer = std::static_pointer_cast<GLUniformBuffer>(uniform_buffer);
    glNamedBufferSubData(gl_uniform_buffer->GetIdentifier(), data_offset, data_size, data);
}

//////////////////////////////////////////////////
////////// Create Methods
//////////////////////////////////////////////////

std::shared_ptr<IMesh> GLRenderHardware::CreateMesh(MeshData* mesh_data)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    uint32 data_size = sizeof(Vertex) * mesh_data->vertices_.size();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data_size), mesh_data->vertices_.data(), GL_STATIC_DRAW);

    constexpr uint32 position_attribute_index = 0;
    constexpr uint32 normal_attribute_index = 1;
    constexpr uint32 uv_attribute_index = 2;
    glEnableVertexAttribArray(position_attribute_index);
    glEnableVertexAttribArray(normal_attribute_index);
    glEnableVertexAttribArray(uv_attribute_index);
    glVertexAttribPointer(position_attribute_index,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position_)));
    glVertexAttribPointer(normal_attribute_index,      3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal_)));
    glVertexAttribPointer(uv_attribute_index,          2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture_coordinate_)));

    uint32 index_count = mesh_data->indices_.size();
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    data_size = sizeof(uint32) * index_count;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data_size), mesh_data->indices_.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::shared_ptr<GLMesh> mesh = std::make_shared<GLMesh>(vao, vertex_buffer, index_buffer, index_count);
    meshes_.push_back(mesh);
    return mesh;
}

std::shared_ptr<IShader> GLRenderHardware::CreateShader(const ShaderStage& shader_stage)
{
    GLuint shader_id;
    switch (shader_stage.type_)
    {
        case IShader::Type::VERTEX_SHADER:
            shader_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case IShader::Type::FRAGMENT_SHADER:
            shader_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case IShader::Type::GEOMETRY_SHADER:
            shader_id = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case IShader::Type::TESSELLATION_CTRL_SHADER:
            shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case IShader::Type::TESSELLATION_EVAL_SHADER:
            shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case IShader::Type::COMPUTE_SHADER:
            shader_id = glCreateShader(GL_COMPUTE_SHADER);
            break;
    }
    const GLchar* sources[] = { shader_stage.source_.c_str() };
    const GLint lengths[] = { static_cast<GLint>(shader_stage.source_.size()) };
    glShaderSource(shader_id, /* Source Count */ 1, sources, lengths);

    glCompileShader(shader_id);
    GLint is_compiled = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        std::string compile_error_message;
        GLint message_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &message_length);
        compile_error_message.resize(message_length);
        glGetShaderInfoLog(shader_id, message_length, &message_length, compile_error_message.data());
        LOG_ERROR(kTitle, "Failed to compile OpenGL shader. Error: " + compile_error_message);
        return nullptr;
    }

    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(shader_id, shader_stage.type_);
    shaders_.push_back(shader_id);
    return shader;
}

std::shared_ptr<IProgram> GLRenderHardware::CreateShaderProgram(const std::vector<std::shared_ptr<IShader>>& shaders)
{
    if (shaders.empty())
    {
        return nullptr;
    }

    GLuint program_id = glCreateProgram();
    for (const std::shared_ptr<IShader>& shader : shaders)
    {
        std::shared_ptr<GLShader> gl_shader = std::static_pointer_cast<GLShader>(shader);
        glAttachShader(program_id, gl_shader->GetIdentifier());
    }

    glLinkProgram(program_id);
    GLint link_result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE)
    {
        std::string linker_error_message;
        GLint message_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &message_length);
        linker_error_message.resize(message_length);
        glGetProgramInfoLog(program_id, message_length, &message_length, linker_error_message.data());
        LOG_ERROR(kTitle, "Failed to link OpenGL program. Error: " + linker_error_message);
        return nullptr;
    }

    for (const std::shared_ptr<IShader>& shader : shaders)
    {
        std::shared_ptr<GLShader> gl_shader = std::static_pointer_cast<GLShader>(shader);
        glDetachShader(program_id, gl_shader->GetIdentifier());
    }

    std::shared_ptr<GLProgram> program = std::make_shared<GLProgram>(program_id);
    programs_.push_back(program_id);
    return program;
}

std::shared_ptr<ITexture> GLRenderHardware::CreateTexture(std::unique_ptr<Image> image)
{
    GLenum texture_unit = GL_TEXTURE0;
    GLenum target = GL_TEXTURE_2D;

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glActiveTexture(texture_unit);
    glBindTexture(target, texture_id);
    GLenum data_format = GL_RGB;

    switch (image->GetPixelFormat())
    {
        case Image::PixelFormat::BGR:
            data_format = GL_BGR;
            break;
        case Image::PixelFormat::RGB:
            data_format = GL_RGB;
            break;
        case Image::PixelFormat::RGBA:
            data_format = GL_RGBA;
            break;
        case Image::PixelFormat::BGRA:
            data_format = GL_BGRA;
            break;
        default:
            LOG_WARN(kTitle, "Cannot create OpenGL texture. Invalid image pixel format.");
            return nullptr;
    }

    glTexImage2D(target,                                 // Target texture)
                 0,                                      // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGBA,                                // Number of color components in the texture.
                 static_cast<GLint>(image->GetWidth()),  // Texture image width
                 static_cast<GLint>(image->GetHeight()), // Texture image height
                 0,                                      // Border. Must be 0.
                 data_format,                            // Format of the pixel data
                 GL_UNSIGNED_BYTE,                       // Data type of the pixel data
                 image->GetData());                      // Pointer to the image data in memory

    glGenerateMipmap(target);

    glActiveTexture(texture_unit);
    glBindTexture(target, 0);

    std::shared_ptr<GLTexture> texture = std::make_shared<GLTexture>(texture_id, target);
    textures_.push_back(texture_id);
    return texture;
}

std::shared_ptr<IUniformBuffer> GLRenderHardware::CreateUniformBuffer(std::string buffer_name, const void* initial_data, uint32 buffer_size)
{
    if (buffer_name.empty())
    {
        LOG_ERROR(kTitle, "Uniform buffer names cannot be empty");
        return nullptr;
    }

    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, initial_data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    std::shared_ptr<GLUniformBuffer> uniform_buffer = std::make_shared<GLUniformBuffer>(buffer_id, buffer_size, std::move(buffer_name));
    uniform_buffers_.push_back(buffer_id);
    return uniform_buffer;
}

//////////////////////////////////////////////////
////////// Frame and Bind Methods
//////////////////////////////////////////////////

void GLRenderHardware::BeginFrame(std::shared_ptr<IFrameBuffer> frame_buffer)
{
    // Bind to default frame buffer if null
    GLuint fbo_id = 0;
    if (frame_buffer)
    {
        std::shared_ptr<GLFrameBuffer> gl_frame_buffer = std::static_pointer_cast<GLFrameBuffer>(frame_buffer);
        fbo_id = gl_frame_buffer->GetIdentifier();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
}

void GLRenderHardware::EndFrame()
{
    // Unbind any existing frame buffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Unbind shader programs
    bound_shader_program_ = nullptr;
    glUseProgram(0);

    // Reset all texture units
    for (uint32 texture_unit = 0; texture_unit < available_texture_unit_index_; ++texture_unit)
    {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Reset available texture units and buffer binding points
    available_texture_unit_index_ = 0;
    available_uniform_buffer_binding_point = 0;
}

void GLRenderHardware::BindShaderProgram(std::shared_ptr<IProgram> shader_program)
{
    assert(shader_program != nullptr);

    bound_shader_program_ = std::static_pointer_cast<GLProgram>(shader_program);
    glUseProgram(bound_shader_program_->GetIdentifier());
    bound_shader_program_->FlushUniforms();
}

void GLRenderHardware::BindTexture(std::shared_ptr<ITexture> texture, std::shared_ptr<ISampler> texture_sampler, const std::string& uniform_name)
{
    assert(texture_sampler != nullptr);

    std::shared_ptr<GLSampler> gl_sampler = std::static_pointer_cast<GLSampler>(texture_sampler);

    // Bind texture to texture unit
    glActiveTexture(GL_TEXTURE0 + available_texture_unit_index_);
    std::shared_ptr<GLTexture> gl_texture = empty_texture_;
    if (texture)
    {
        gl_texture = std::static_pointer_cast<GLTexture>(texture);
    }
    glBindTexture(gl_texture->GetTarget(), gl_texture->GetIdentifier());

    // Bind texture sampler to texture unit
    glBindSampler(available_texture_unit_index_, gl_sampler->GetIdentifier());

    // Map shader uniform name to texture unit
    glUniform1i(glGetUniformLocation(bound_shader_program_->GetIdentifier(), uniform_name.c_str()), static_cast<int32>(available_texture_unit_index_));

    // Increment to the next available texture unit
    ++available_texture_unit_index_;
}

void GLRenderHardware::BindUniformBuffer(std::shared_ptr<IUniformBuffer> uniform_buffer)
{
    assert(uniform_buffer != nullptr);
    assert(bound_shader_program_ != nullptr);

    std::shared_ptr<GLUniformBuffer> gl_uniform_buffer = std::static_pointer_cast<GLUniformBuffer>(uniform_buffer);

    glBindBuffer(GL_UNIFORM_BUFFER, gl_uniform_buffer->GetIdentifier());

    // Retrieve uniform buffer index from name
    const std::string& uniform_name = gl_uniform_buffer->GetName();
    GLuint uniform_block_index = glGetUniformBlockIndex(bound_shader_program_->GetIdentifier(), uniform_name.c_str());

    // Bind buffer to binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, available_uniform_buffer_binding_point, gl_uniform_buffer->GetIdentifier());

    // Bind program uniform buffer index to binding point
    glUniformBlockBinding(bound_shader_program_->GetIdentifier(), uniform_block_index, available_uniform_buffer_binding_point);

    // Increment to the next available binding point
    ++available_uniform_buffer_binding_point;
}

//////////////////////////////////////////////////
////////// Draw Method
//////////////////////////////////////////////////

void GLRenderHardware::DrawMesh(std::shared_ptr<IMesh> mesh)
{
    assert(bound_shader_program_ != nullptr);

    std::shared_ptr<GLMesh> gl_mesh = std::static_pointer_cast<GLMesh>(mesh);
    glBindVertexArray(gl_mesh->GetVAOIdentifier());
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(gl_mesh->GetIndexDataSize()), GL_UNSIGNED_INT, nullptr);
}

} // namespace zero::render