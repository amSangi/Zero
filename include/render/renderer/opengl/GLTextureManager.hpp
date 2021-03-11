#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "render/renderer/ITextureManager.hpp"
#include "render/renderer/opengl/GLSampler.hpp"
#include "render/renderer/opengl/GLTexture.hpp"

namespace zero::render
{
    class GLTextureManager final : public ITextureManager
    {
    public:
        GLTextureManager();
        virtual ~GLTextureManager();

        uint32 GetTextureUnitCount() const override;

        void Initialize() override;
        bool LoadTexture(const std::string& image_name, const std::string& filename) override;

        void ReleaseTextures() override;

        ITexture* GetTexture(const std::string& image_name) override;

        std::shared_ptr<ISampler> GetDiffuseMapSampler() override;
        std::shared_ptr<ISampler> GetShadowMapSampler() override;

        const std::vector<std::shared_ptr<IRenderTarget>>& GetShadowMapRenderTargets() override;
        const std::vector<std::shared_ptr<ITexture>>& GetShadowMapTextures() override;

    private:
        void SetupEmptyTexture();
        void SetupSamplers();
        void SetupShadowMapTextures();
        void SetupShadowMapRenderTargets();

        std::unordered_map<std::string, std::shared_ptr<GLTexture>> texture_map_;
        std::vector<std::shared_ptr<IRenderTarget>> shadow_map_render_targets_;
        std::vector<std::shared_ptr<ITexture>> shadow_map_textures_;
        std::shared_ptr<GLSampler> diffuse_map_sampler_;
        std::shared_ptr<GLSampler> shadow_map_sampler_;
        std::shared_ptr<GLTexture> empty_texture_;

    }; // class GLTextureManager

} // namespace zero::render