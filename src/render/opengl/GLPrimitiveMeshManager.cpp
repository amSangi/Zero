#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/opengl/GLMesh.hpp"
#include "render/Components.hpp"

using namespace zero::render;

GLPrimitiveMeshManager::GLPrimitiveMeshManager()
: box_mesh_(nullptr)
, sphere_mesh_(nullptr)
, plane_mesh_(nullptr)
, cone_mesh_(nullptr)
, cylinder_mesh_(nullptr)
, torus_mesh_(nullptr)
{}

void GLPrimitiveMeshManager::LoadPrimitives() {
    // Cache default primitives
    PrimitiveInstance instance{};
    instance.Set(render::Box());
    box_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(render::Sphere());
    sphere_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(render::Plane());
    plane_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(render::Cone());
    cone_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(render::Cylinder());
    cylinder_mesh_ = GLPrimitiveGenerator::Generate(instance);

    instance.Set(render::Torus());
    torus_mesh_ = GLPrimitiveGenerator::Generate(instance);
}

void GLPrimitiveMeshManager::ClearPrimitives() {
    box_mesh_.reset();
    sphere_mesh_.reset();
    plane_mesh_.reset();
    cone_mesh_.reset();
    cylinder_mesh_.reset();
    torus_mesh_.reset();
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::GetPrimitiveMesh(PrimitiveInstance primitive_instance) const {
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

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Box& box) const {
    if (box == Box()) {
        return box_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(box);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Sphere& sphere) const {
    if (sphere == Sphere()) {
        return sphere_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(sphere);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Plane& plane) const {
    if (plane == Plane()) {
        return plane_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(plane);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Cone& cone) const {
    if (cone == Cone()) {
        return cone_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(cone);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Cylinder& cylinder) const {
    if (cylinder == Cylinder()) {
        return cylinder_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(cylinder);
    return GLPrimitiveGenerator::Generate(instance);
}

std::shared_ptr<GLMesh> GLPrimitiveMeshManager::Generate(const Torus& torus) const {
    if (torus == Torus()) {
        return torus_mesh_;
    }
    PrimitiveInstance instance{};
    instance.Set(torus);
    return GLPrimitiveGenerator::Generate(instance);
}

