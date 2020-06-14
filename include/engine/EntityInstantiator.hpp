#pragma once

#include <string>
#include "component/Component.hpp"
#include "core/IEntityInstantiator.hpp"

namespace zero
{
    // Forward declaration
    class Light;
    class PrimitiveInstance;

namespace render
{
    // Forward declaration
    class RenderSystem;
} // namespace render

    /**
     * @see IEntityInstantiator
     */
    class EntityInstantiator : public IEntityInstantiator
    {
    public:
        explicit EntityInstantiator(render::RenderSystem* render_system);
        ~EntityInstantiator() override = default;

        /**
         * @see IEntityInstantiator::InstantiateModel
         */
        Entity InstantiateModel(const std::string& model_filename) override;

        /**
         * @see IEntityInstantiator::InstantiatePrimitive
         */
        Entity InstantiatePrimitive(const PrimitiveInstance& primitive) override;

        /**
         * @see IEntityInstantiator::InstantiateLight
         */
        Entity InstantiateLight(const Light& light, Entity entity) override;

    private:
        render::RenderSystem* render_system_;

    }; // class EntityInstantiator

} // namespace zero