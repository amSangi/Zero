#pragma once

#include "core/Component.hpp"

namespace zero::render {

    class GLModel;
    class GLInstantiator {
    public:

        /**
         * @brief Create a new entity based on a GLModel.
         *
         * Constructs an entity with a Transform, Volume, Material, and MeshInstance components.
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

    }; // class GLInstantiator

} // namespace zero::render
