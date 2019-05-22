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

    /**
     * @brief The RenderingSystem uses transform, mesh, material, and camera components
     *   to render a scene. It is responsible for all windowing/rendering tasks.
     *   The list of tasks include:
     *      - Creating/Destroying the window and OpenGL contexts
     *      - Propagating parent-child transforms
     *      - Propagating bounding volume hierarchies
     *      - Creating/Updating/Deleting renderable objects from the transform, mesh, and material components
     *      - Culling (View volume and occlusion)
     *      - Drawing renderables via the Renderer
     */
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