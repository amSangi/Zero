#include "render/renderer/opengl/GLModelManager.hpp"
#include "render/renderer/opengl/GLPrimitiveGenerator.hpp"

namespace zero::render
{

Box GLModelManager::kDefaultBox{};
Sphere GLModelManager::kDefaultSphere{};
Plane GLModelManager::kDefaultPlane{};
Cone GLModelManager::kDefaultCone{};
Cylinder GLModelManager::kDefaultCylinder{};
Torus GLModelManager::kDefaultTorus{};

GLModelManager::GLModelManager()
: model_map_()
{
}

GLModelManager::~GLModelManager()
{
}

void GLModelManager::Initialize()
{
    // Cache default primitives
    PrimitiveInstance instance{};
    instance.Set(Box());
    box_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(Sphere());
    sphere_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(Plane());
    plane_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(Cone());
    cone_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(Cylinder());
    cylinder_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(Torus());
    torus_mesh_ = GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<Model> GLModelManager::CreateModel(const std::string& model_name,
                                                   std::unique_ptr<Mesh> mesh,
                                                   const Transform& transform,
                                                   const Material& material,
                                                   const Volume& volume,
                                                   const ModelInstance& model_instance)
{
    auto gl_mesh = std::make_shared<GLMesh>();
    gl_mesh->Initialize(std::move(mesh));

    std::shared_ptr<GLModel> model = std::make_shared<GLModel>(gl_mesh,
                                                               transform,
                                                               material,
                                                               volume,
                                                               model_instance);
    // TODO: Fix Clion cast warning
    model_map_.emplace(model_name, model);
    return static_pointer_cast<Model>(model);
}

void GLModelManager::ClearModels()
{
    model_map_.clear();
}

void GLModelManager::ClearPrimitives()
{
    box_mesh_.reset();
    sphere_mesh_.reset();
    plane_mesh_.reset();
    cone_mesh_.reset();
    cylinder_mesh_.reset();
    torus_mesh_.reset();
}

std::shared_ptr<Model> GLModelManager::GetModel(const std::string& model_name)
{
    auto model_search = model_map_.find(model_name);
    if (model_search == model_map_.end())
    {
        return nullptr;
    }
    return model_search->second;
}

std::shared_ptr<Model> GLModelManager::GetModel(const ModelInstance& model_instance)
{
    auto model = GetModel(model_instance.model_name_);
    if (!model)
    {
        return nullptr;
    }

    if (model->GetModelInstance().node_name_ == model_instance.node_name_)
    {
        return model;
    }

    // Search for the correct child node
    return model->FindChild(model_instance.node_name_);
}

std::shared_ptr<IMesh> GLModelManager::GetPrimitive(const PrimitiveInstance& primitive_instance)
{
    switch (primitive_instance.GetType())
    {
        case PrimitiveInstance::Type::SPHERE:
            return Generate(primitive_instance.GetSphere());
        case PrimitiveInstance::Type::PLANE:
            return Generate(primitive_instance.GetPlane());
        case PrimitiveInstance::Type::CONE:
            return Generate(primitive_instance.GetCone());
        case PrimitiveInstance::Type::CYLINDER:
            return Generate(primitive_instance.GetCylinder());
        case PrimitiveInstance::Type::TORUS:
            return Generate(primitive_instance.GetTorus());
        default:
            return Generate(primitive_instance.GetBox());
    }
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Box& box) const
{
    if (box == kDefaultBox)
    {
        return box_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(box);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Sphere& sphere) const
{
    if (sphere == kDefaultSphere)
    {
        return sphere_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(sphere);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Plane& plane) const
{
    if (plane == kDefaultPlane)
    {
        return plane_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(plane);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Cone& cone) const
{
    if (cone == kDefaultCone)
    {
        return cone_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(cone);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Cylinder& cylinder) const
{
    if (cylinder == kDefaultCylinder)
    {
        return cylinder_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(cylinder);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLModelManager::Generate(const Torus& torus) const
{
    if (torus == kDefaultTorus)
    {
        return torus_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(torus);
    return GLPrimitiveGenerator::Generate(instance);
}


} // namespace zero::render