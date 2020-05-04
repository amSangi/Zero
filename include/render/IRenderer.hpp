#pragma once

#include <string>
#include <entt/entt.hpp>
#include "core/RenderSystemConfig.hpp"
#include "render/IModel.hpp"

namespace zero::render {

    /**
     * @brief Draw primitives and objects onto the current active rendering context
     */
    class IRenderer {
    public:

        virtual ~IRenderer() = default;

        /**
         * @brief Initialize all the texture images, shaders, and models
         * @param config the render system configuration details
         */
        virtual void Initialize(const RenderSystemConfig& config) = 0;

        /**
         * @brief Render all entities in the registry with Transform, Volume, Material, and either
         * ModelInstance or PrimitiveInstance components using the active cameras in the world.
         * @param registry the registry containing all of the entities and their components
         */
        virtual void Render(const entt::registry& registry) = 0;

        /**
         * @brief Perform post-rendering cleanup
         * @param registry the registry containing all of the entities and their components
         */
        virtual void PostRender(entt::registry& registry) = 0;

        /**
         * @brief Clear all texture images, shaders, and models
         */
        virtual void ShutDown() = 0;

        /**
         * @brief Get the IModel of a 3D model given the filename
         * @param model the fully qualified 3D filename
         * @return the IModel. Nullptr if it does not exist.
         */
        [[nodiscard]] virtual std::weak_ptr<IModel> GetModel(const std::string& model) = 0;

    }; // class IRenderer

} // namespace zero::render