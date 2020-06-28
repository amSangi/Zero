#pragma once

#include <string>
#include "engine/RenderSystemConfig.hpp"
#include "render/IModel.hpp"

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
         */
        virtual void Initialize() = 0;

        /**
         * @brief Render all entities in the registry with Transform, Volume, Material, and either
         * ModelInstance or PrimitiveInstance components using the active cameras in the world.
         */
        virtual void Render() = 0;

        /**
         * @brief Perform post-rendering cleanup
         */
        virtual void PostRender() = 0;

        /**
         * @brief Clear all texture images, shaders, and models
         */
        virtual void ShutDown() = 0;

        /**
         * @brief Get the IModel
         * @param model the model filename
         * @return the IModel. Nullptr if it does not exist.
         */
        [[nodiscard]] virtual std::weak_ptr<IModel> GetModel(const std::string& model) = 0;

    }; // class IRenderer

} // namespace zero::render