#pragma once

#include "core/Component.hpp"

namespace zero::render {

    class GLModel;
    class GLInstantiator {
    public:

        GLInstantiator() = default;

        /**
         * @brief Create a new entity based on a GLModel.
         *
         * Constructs an entity with a Transform, Volume, Material, and MeshInstance components.
         * If the model contains sub-children, they are created and added to the transform component.
         *
         * @param registry the registry containing all of the entities and their components
         * @param gl_model the GLModel to instantiate
         * @return the root entity associated with the 3D GLModel. NullEntity if an error occurred.
         */
        [[nodiscard]] Component::Entity InstantiateModel(entt::registry& registry,
                                                         std::shared_ptr<GLModel> gl_model);

    }; // class GLInstantiator

} // namespace zero::render
