#pragma once

#include "core/Component.hpp"
#include "render/Components.hpp"

namespace zero::render {

    class GLModel;
    class GLInstantiator {
    public:

        GLInstantiator() = delete;

        /**
         * @brief Create a new entity based on a GLModel.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         * If the model contains sub-children, they are created and added to the transform component.
         *
         * @param registry the registry containing all of the entities and their components
         * @param gl_model the GLModel to instantiate
         * @param parent the parent of the entity that is instantiated
         * @return the root entity associated with the 3D GLModel. NullEntity if an error occurred.
         */
        [[nodiscard]] static Component::Entity InstantiateModel(entt::registry& registry,
                                                                const std::shared_ptr<GLModel>& gl_model,
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
    }; // class GLInstantiator

} // namespace zero::render
