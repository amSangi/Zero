#pragma once

#include <string>
#include "component/Component.hpp"

namespace zero
{
    // Forward declarations
    class Light;
    class PrimitiveInstance;

    class IEntityInstantiator
    {
    public:
        IEntityInstantiator() = default;
        virtual ~IEntityInstantiator() = default;

        /**
         * @brief Create a new entity based on a 3D model.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        [[nodiscard]] virtual Entity InstantiateModel(const std::string& model_filename) = 0;

        /**
         * @brief Create a new entity based on a primitive shape.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        [[nodiscard]] virtual Entity InstantiatePrimitive(const PrimitiveInstance& primitive) = 0;

        /**
         * @brief Create a new light entity or attach a light component to an existing entity
         *
         * If the entity is null, constructs an entity with Transform
         * and either DirectionalLight, PointLight, or SpotLight components.
         *
         * @param light the light component data container
         * @param entity the entity to attach the light to. Null Entity if a new entity should be created.
         * @return the light entity instance
         */
        virtual Entity InstantiateLight(const Light& light, Entity entity) = 0;
    };

} // namespace zero