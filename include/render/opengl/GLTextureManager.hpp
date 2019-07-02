#pragma once

#include "render/Image.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/ITextureManager.hpp"

namespace zero::render {

    class GLTextureManager : public ITextureManager {
    public:


        GLTextureManager();

        ~GLTextureManager() override = default;

        uint8 GetTextureUnitCount() const override;

        /**
         * @brief Set the texture sampler for a given texture unit
         * @param sampler the texture sampler to assign
         * @param texture_unit the texture unit to assign the sampler to. Range from [0, GetTextureUnitCount).
         */
        void SetSampler(const std::shared_ptr<GLSampler>& sampler, uint8 texture_unit);

        bool InitializeImage(const std::string& filename) override;

        std::shared_ptr<GLTexture> CreateTexture(const std::string& filename, uint8 index); 

    protected:
        GLTexture* CreateRawTexture(const std::string& filename, uint8 index) override;

    private:
        std::unordered_map<std::string, std::shared_ptr<Image>> image_map_;

    }; // class GLTextureManager

} // namespace zero::render