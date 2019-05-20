#pragma once

#include <memory>
#include <vector>
#include "core/System.hpp"
#include "ICuller.hpp"
#include "IRenderer.hpp"
#include "RenderConfig.hpp"
#include "SceneGraph.hpp"
#include "Window.hpp"

namespace zero::render {

    class RenderSystem : public System {
    public:
        RenderSystem(RenderConfig config);
        ~RenderSystem() override;
        void Initialize() override;
        void Update(float dt) override;
        void ShutDown() override;

    private:
        void Propagate();
        std::vector<std::shared_ptr<ICuller>> GetCullers();


        RenderConfig config_;
        Window window_;
        SceneGraph scene_graph_;
        std::unique_ptr<IRenderer> renderer_;

    }; // class RenderSystem

} // namespace zero::render