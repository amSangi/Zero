#pragma once

#include "core/NonCopyable.hpp"
#include "render/renderer/IRenderTarget.hpp"
#include "render/renderer/ISampler.hpp"
#include "render/renderer/ITexture.hpp"

namespace zero::render
{

    class ITextureManager : public NonCopyable
    {
    public:
        virtual ~ITextureManager() = default;
        virtual uint32 GetTextureUnitCount() const = 0;

        virtual void Initialize() = 0;
        virtual bool LoadTexture(const std::string& image_name, const std::string& filename) = 0;

        virtual void ReleaseTextures() = 0;

        virtual std::shared_ptr<ISampler> GetDiffuseMapSampler() = 0;
        virtual std::shared_ptr<ISampler> GetShadowMapSampler() = 0;

        virtual ITexture* GetTexture(const std::string& image_name) = 0;

        virtual const std::vector<std::shared_ptr<IRenderTarget>>& GetShadowMapRenderTargets() = 0;
        virtual const std::vector<std::shared_ptr<ITexture>>& GetShadowMapTextures() = 0;

    }; // interface IUniformManager

} // namespace zero::render