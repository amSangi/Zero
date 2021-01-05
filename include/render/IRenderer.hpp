#pragma once

#include <string>
#include "engine/RenderSystemConfig.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    /**
     * @brief Draw primitives and objects onto the current active rendering context
     */
    class IRenderer
    {
    public:

        virtual ~IRenderer() = default;

        /**
         * @brief Initialize all the texture images, shaders, and models
         * @param config The RenderSystem configuration information
         */
        virtual void Initialize(const RenderSystemConfig& config) = 0;

        /**
         * @brief Render all entities in the registry with Transform, Volume, Material, and either
         * ModelInstance or PrimitiveInstance components using the active cameras in the world.
         * @param time_delta updated timing information since the last engine tick
         */
        virtual void Render(const TimeDelta& time_delta) = 0;

        /**
         * @brief Perform post-rendering cleanup
         */
        virtual void PostRender() = 0;

        /**
         * @brief Clear all texture images, shaders, and models
         */
        virtual void ShutDown() = 0;

        /**
         * @brief Get the Model
         * @param model the model filename
         * @return the Model. Nullptr if it does not exist.
         */
        [[nodiscard]] virtual std::weak_ptr<Model> GetModel(const std::string& model) = 0;

    }; // class IRenderer

} // namespace zero::render