#pragma once

#include <string>
#include "component/Component.hpp"
#include "component/Light.hpp"
#include "component/SkyDome.hpp"
#include "component/PrimitiveInstance.hpp"

namespace zero
{
    class IEntityInstantiator
    {
    public:
        IEntityInstantiator() = default;
        virtual ~IEntityInstantiator() = default;

        /**
         * @brief Create a new entity based on a model.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param model the model name
         * @param parent_entity the parent entity of the newly instantiated model
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        [[nodiscard]] virtual Entity InstantiateModel(const std::string& model_name, Entity parent_entity) = 0;

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

        /**
         * @brief Create a new sky dome entity
         * @param registry the registry containing all of the entities and their components
         * @param sky_dome the sky dome component to instantiate into an entity
         * @return the sky dome entity instance
         */
        [[nodiscard]] virtual Entity InstantiateSkyDome(const SkyDome& sky_dome) = 0;
    };

} // namespace zero