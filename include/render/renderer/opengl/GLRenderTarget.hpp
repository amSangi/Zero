#pragma once

#include "render/renderer/IRenderTarget.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLRenderTarget : public IRenderTarget
    {
    public:
        GLRenderTarget();
        ~GLRenderTarget() override;

        void Initialize(IRenderTarget::Format format, uint32 width, uint32 height, std::shared_ptr<ITexture> texture) override;

        /**
         * @brief Get the identifier for the frame buffer
         * @return the id
         */
        [[nodiscard]] GLuint GetNativeIdentifier() const;

    private:
        GLuint fbo_id_;

    }; // class GLRenderTarget

} // namespace zero::render