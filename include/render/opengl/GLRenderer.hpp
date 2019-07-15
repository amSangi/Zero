#pragma once

#include <entt.hpp>
#include "render/IRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLTextureManager.hpp"

namespace zero::render {

    class GLRenderer : public IRenderer {
    public:

        GLRenderer() = default;

        ~GLRenderer() override = default;

        void Initialize() override;

        void Render(const entt::registry& registry, float dt) override;

        void PostRender(entt::registry& registry) override;

        void ShutDown() override;
    }; // class GLRenderer

} // namespace zero::render