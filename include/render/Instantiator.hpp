#pragma once

#include "core/Component.hpp"

namespace zero::render {

    // Forward declarations
    class IModel;
    class Light;
    class PrimitiveInstance;

    class Instantiator {
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
        [[nodiscard]] static Component::Entity InstantiateModel(entt::registry& registry,
                                                                const std::shared_ptr<IModel>& model,
                                                                Component::Entity parent = Component::NullEntity);

        /**
         * @brief Create a new entity based on a primitive.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param primitive the primitive instance data.
         * @return the entity associated with the new instantiated primitive.
         */
        [[nodiscard]] static Component::Entity InstantiatePrimitive(entt::registry& registry,
                                                                    const PrimitiveInstance& primitive);

        /**
         * @brief Create a new light entity
         *
         * Constructs an entity with Transform and a light components.
         * The light component can be either a DirectionalLight, PointLight, or SpotLight.
         *
         * @param registry the registry containing all of the entities and their components
         * @param light the light component container
         * @return the light entity instance
         */
        [[nodiscard]] static Component::Entity InstantiateLight(entt::registry& registry, const Light& light);

    }; // class Instantiator

} // namespace zero::render
