#pragma once

#include "component/Light.hpp"
#include "component/PrimitiveInstance.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    /**
     * @brief Instantiate a game entity with a set of components
     */
    class EntityFactory
    {
    public:

        EntityFactory() = delete;

        /**
         * @brief Create a new entity based on a Model.
         *
         * Constructs an entity with Transform, Volume. Not all entities in the hierarchy will have Mesh and Material components.
         * If the model contains sub-children, they are created and added to each transform component.
         *
         * @param registry the registry containing all of the entities and their components
         * @param model the Model to instantiate
         * @param parent_entity the parent of the entity that is instantiated
         * @return the root parent entity of the instantiated prototype. NullEntity if an error occurred.
         */
        [[nodiscard]] static Entity InstantiateModel(entt::registry& registry, const std::shared_ptr<Model>& model, Entity parent_entity);

        /**
         * @brief Create a new entity based on a primitive.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param registry the registry containing all of the entities and their components
         * @param primitive the primitive instance data.
         * @return the entity associated with the new instantiated primitive.
         */
        [[nodiscard]] static Entity InstantiatePrimitive(entt::registry& registry, uint32 mesh_id, const PrimitiveInstance& primitive);

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
        [[nodiscard]] static Entity InstantiateLight(entt::registry& registry, const Light& light, Entity entity);

	private:
		static Entity InstantiateNode(entt::registry& registry, const std::shared_ptr<Node>& node, const std::vector<GeometryData>& geometry_data_list, Entity parent_entity);

    }; // class EntityFactory

} // namespace zero::render
