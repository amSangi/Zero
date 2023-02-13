#pragma once

#include "render/IRenderView.hpp"
#include "render/renderer/IDrawCall.hpp"
#include "render/renderer/IRenderHardware.hpp"

namespace zero::render
{

    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;

        /**
         * @brief Setup the render pass for the first time
         * @param rhi the render hardware interface to use for the creation of any rendering resources
         * @param camera_uniform the camera uniform buffer to use during rendering
         */
        virtual void Initialize(IRenderHardware* rhi, std::shared_ptr<IUniformBuffer> camera_uniform) = 0;

        /**
         * @brief Submit a new draw call for sorting and execution
         * @param draw_call the new draw call to manage
         */
        virtual void Submit(std::unique_ptr<IDrawCall> draw_call) = 0;

        /**
         * @brief Sort the draw calls
         */
        virtual void Sort() = 0;

        /**
         * @brief Execute the draw calls
         * @param render_view the render view associated with this render
         * @param rhi the render hardware interface to use for rendering
         */
        virtual void Render(IRenderView* render_view, IRenderHardware* rhi) = 0;

        /**
         * @brief Clear the draw calls
         */
        virtual void ClearDrawCalls() = 0;
    };

} // namespace zero::render