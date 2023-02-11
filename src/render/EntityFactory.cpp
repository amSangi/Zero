#include "render/EntityFactory.hpp"
#include "component/Material.hpp"
#include "component/Mesh.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "math/Box.hpp"

namespace zero::render
{

Entity EntityFactory::InstantiateModel(entt::registry& registry, const std::shared_ptr<Model>& model, Entity parent_entity)
{
    return InstantiateNode(registry, model->root_node_, model->geometries_, parent_entity);
}

Entity EntityFactory::InstantiateNode(entt::registry& registry, const std::shared_ptr<Node>& node, const std::vector<GeometryData>& geometry_data_list, Entity parent_entity)
{
	Entity entity = registry.create();
	for (const std::shared_ptr<Node>& child_node: node->child_nodes_)
	{
		InstantiateNode(registry, child_node, geometry_data_list, entity);
	}
	registry.emplace<Volume>(entity, node->volume_);
	registry.emplace<Transform>(entity, Transform::FromMatrix4x4(node->transform_));

	if (node->geometry_indices_.size() == 1)
	{
		// Directly assign Mesh and Material components  to entity
		const GeometryData& geometry_data = geometry_data_list[0];
		registry.emplace<Material>(entity, *geometry_data.material_);
		Mesh& mesh = registry.emplace<Mesh>(entity);
		mesh.mesh_id_ = geometry_data_list[0].geometry_id_;
	}
	else
	{
		// Instantiate each geometry and add it as a child
		for (uint32 geometry_index: node->geometry_indices_)
		{
			const GeometryData& geometry_data = geometry_data_list[geometry_index];

			Entity geometry_entity = registry.create();
			registry.emplace<Material>(geometry_entity, *geometry_data.material_);
			Mesh& geometry_mesh = registry.emplace<Mesh>(geometry_entity);
			geometry_mesh.mesh_id_ = geometry_data.geometry_id_;
			registry.emplace<Volume>(geometry_entity, geometry_data.volume_);

			Transform& geometry_entity_transform = registry.emplace<Transform>(geometry_entity);
			Transform& entity_transform = registry.get<Transform>(entity);

			entity_transform.children_.push_back(geometry_entity);
			geometry_entity_transform.parent_ = entity;
		}
	}

	Transform& parent_transform = registry.get<Transform>(parent_entity);
	Transform& entity_transform = registry.get<Transform>(entity);
	parent_transform.children_.push_back(entity);
	entity_transform.parent_ = parent_entity;

	return entity;
}

Entity EntityFactory::InstantiatePrimitive(entt::registry& registry, uint32 mesh_id, const PrimitiveInstance& primitive)
{
	// TODO: Refactor Primitive Instance
	Entity entity = registry.create();
    Transform transform{};
    Volume volume{};
    // Set transform/volume values
    switch (primitive.GetType())
    {
        case PrimitiveInstance::Type::BOX:
        {
            Box box = primitive.GetBox();
            math::Box math_box{math::Vec3f::Zero(), math::Vec3f(static_cast<float>(box.width_),
																static_cast<float>(box.height_),
																static_cast<float>(box.depth_))};
            volume.bounding_volume_.center_ = math_box.Center();
            volume.bounding_volume_.radius_ = math_box.max_.Magnitude() * 0.5F;
            transform.scale_ = math_box.max_;
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            Cone cone = primitive.GetCone();
            float half_height = static_cast<float>(cone.height_) * 0.5F;
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + cone.radius_ * cone.radius_);
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            Cylinder cylinder = primitive.GetCylinder();
            float half_height = static_cast<float>(cylinder.height_) * 0.5F;
            float largest_radius = math::Max(cylinder.bottom_radius_, cylinder.top_radius_);
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + largest_radius * largest_radius);
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            Plane plane = primitive.GetPlane();
            float half_width = static_cast<float>(plane.width_) * 0.5F;
            float half_height = static_cast<float>(plane.height_) * 0.5F;
            volume.bounding_volume_.center_ = math::Vec3f(half_width,
                                                          0.0F,
                                                          half_height);
            volume.bounding_volume_.radius_ = half_width + half_height;
            break;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            // Volume slightly larger
            volume.bounding_volume_.radius_ = 1.05F;
            break;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            Torus torus = primitive.GetTorus();
            // Volume slightly larger
            volume.bounding_volume_.radius_ = (torus.radius_ + torus.tube_radius_) + 0.05F;
            break;
        }
    }

    registry.emplace<Volume>(entity, volume);
    // Use default shaders
    registry.emplace<Material>(entity, Material{});
    registry.emplace<PrimitiveInstance>(entity, primitive);
    registry.emplace<Transform>(entity, transform);
	Mesh& mesh = registry.emplace<Mesh>(entity);
    mesh.mesh_id_ = mesh_id;
	return entity;
}

Entity EntityFactory::InstantiateLight(entt::registry& registry, const Light& light, Entity entity)
{
    Entity entity_to_attach = entity;
    if (!registry.valid(entity_to_attach))
    {
        entity_to_attach = registry.create();
    }

    switch (light.GetType())
    {
        case Light::Type::DIRECTIONAL:
        {
            registry.emplace<DirectionalLight>(entity_to_attach, light.GetDirectionalLight());
            break;
        }
        case Light::Type::POINT:
        {
            registry.emplace<PointLight>(entity_to_attach, light.GetPointLight());
            break;
        }
        case Light::Type::SPOT:
        {
            registry.emplace<SpotLight>(entity_to_attach, light.GetSpotLight());
            break;
        }
    }

    // Assign a new transform if one does not exist
    auto transform = registry.try_get<Transform>(entity_to_attach);
    if (transform == nullptr)
    {
        registry.emplace<Transform>(entity_to_attach, Transform{});
    }

    return entity_to_attach;
}

} // namespace zero::render