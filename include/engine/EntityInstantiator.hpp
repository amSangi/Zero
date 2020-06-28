#pragma once

#include <string>
#include "component/Component.hpp"
#include "core/IEntityInstantiator.hpp"

namespace zero
{
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
        EntityInstantiator(entt::registry& registry, render::RenderSystem* render_system);
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

        /**
         * @see IEntityInstantiator::InstantiateSkyDome
         */
        Entity InstantiateSkyDome(const SkyDome& sky_dome) override;

    private:
        entt::registry& registry_;
        render::RenderSystem* render_system_;

    }; // class EntityInstantiator

} // namespace zero