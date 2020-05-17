#pragma once

#include "core/Component.hpp"

namespace zero::render
{

    // Forward declarations
    class IModel;
    class Light;
    class PrimitiveInstance;

    class Instantiator
    {
    public:

        Instantiator() = delete;

        /**
         * @brief Create a new entity based on a IModel.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         * If the model contains sub-children, they are created and added to the transform component.
         *
         * @param registry the registry containing all of the entities and their components
         * @param model the IModel to instantiate
         * @param parent the parent of the entity that is instantiated
         * @return the root entity associated with the 3D GLModel. NullEntity if an error occurred.
         */
        [[nodiscard]] static Entity InstantiateModel(entt::registry& registry,
                                                     const std::shared_ptr<IModel>& model,
                                                     Entity parent = NullEntity);

        /**
         * @brief Create a new entity based on a primitive.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param primitive the primitive instance data.
         * @return the entity associated with the new instantiated primitive.
         */
        [[nodiscard]] static Entity InstantiatePrimitive(entt::registry& registry, const PrimitiveInstance& primitive);

        /**
         * @brief Create a new light entity or attach a light component to an existing entity
         *
         * If the entity exists, it attaches a light component to that entity.
         * Otherwise, constructs an entity with Transform and a light component.
         * The light component can be either a DirectionalLight, PointLight, or SpotLight.
         *
         * @param registry the registry containing all of the entities and their components
         * @param light the light component container
         * @param entity the entity to attach the light to. Null Entity if a new entity should be created.
         * @return the light entity instance
         */
        static Entity InstantiateLight(entt::registry& registry, const Light& light, Entity entity);

    }; // class Instantiator

} // namespace zero::render
