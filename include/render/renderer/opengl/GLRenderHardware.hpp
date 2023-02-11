#pragma once

#include "render/renderer/IRenderHardware.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{
    class GLMesh;
    class GLProgram;
    class GLSampler;
    class GLShader;
    class GLTexture;
    class GLUniformBuffer;

    class GLRenderHardware : public IRenderHardware
    {
    public:
        GLRenderHardware();
        ~GLRenderHardware() override = default;
        void Initialize() override;
        void Shutdown() override;

        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)  override;
        void SetFillMode(FillMode fill_mode) override;
        void SetCullMode(CullMode cull_mode) override;

        void SetClearColor(const math::Vec4f& color) override;
        void Clear() override;

        std::shared_ptr<ISampler> GetDiffuseMapSampler() override;
        std::shared_ptr<ISampler> GetShadowMapSampler() override;

        const std::vector<std::shared_ptr<ITexture>>& GetShadowMapTextures() override;
        const std::vector<std::shared_ptr<IFrameBuffer>>& GetShadowMapFrameBuffers() override;

        void UpdateUniformData(std::shared_ptr<IUniformBuffer> uniform_buffer, const void* data, uint32 data_size, uint32 data_offset) override;

        std::shared_ptr<IMesh> CreateMesh(MeshData* mesh_data) override;
        std::shared_ptr<IShader> CreateShader(const ShaderStage& shader_stage) override;
        std::shared_ptr<IProgram> CreateShaderProgram(const std::vector<std::shared_ptr<IShader>>& shaders) override;
        std::shared_ptr<ITexture> CreateTexture(std::unique_ptr<Image> image) override;
        std::shared_ptr<IUniformBuffer> CreateUniformBuffer(std::string buffer_name, const void* initial_data, uint32 buffer_size) override;

        void BeginFrame(std::shared_ptr<IFrameBuffer> frame_buffer) override;
        void EndFrame() override;

        void BindShaderProgram(std::shared_ptr<IProgram> shader_program) override;
        void BindTexture(std::shared_ptr<ITexture> texture, std::shared_ptr<ISampler> texture_sampler, const std::string& uniform_name) override;
        void BindUniformBuffer(std::shared_ptr<IUniformBuffer> uniform_buffer) override;

        void DrawMesh(std::shared_ptr<IMesh> mesh) override;

    private:
        /**
         * @brief The log title
         */
        static const char* kTitle;
        std::shared_ptr<GLSampler> diffuse_map_sampler_;
        std::shared_ptr<GLSampler> shadow_map_sampler_;
        std::vector<std::shared_ptr<GLMesh>> meshes_;
        std::vector<GLuint> shaders_;
        std::vector<GLuint> programs_;
        std::vector<GLuint> textures_;
        std::vector<GLuint> uniform_buffers_;
        std::shared_ptr<GLProgram> bound_shader_program_;
        std::vector<std::shared_ptr<ITexture>> shadow_map_textures_;
        std::vector<std::shared_ptr<IFrameBuffer>> shadow_map_frame_buffers_;
        uint32 available_texture_unit_index_;
        uint32 available_uniform_buffer_binding_point;
    }; // class GLRenderHardware

} // namespace zero::render