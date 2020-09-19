#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/opengl/GLMesh.hpp"
#include "render/MeshGenerator.hpp"
#include "component/PrimitiveInstance.hpp"

namespace zero::render
{

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const PrimitiveInstance& primitive)
{
    switch (primitive.GetType())
    {
        case PrimitiveInstance::Type::BOX:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GenerateBox());
            mesh->Initialize();
            return mesh;
        }
        case PrimitiveInstance::Type::CONE:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GenerateCone(primitive.GetCone()));
            mesh->Initialize();
            return mesh;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GenerateCylinder(primitive.GetCylinder()));
            mesh->Initialize();
            return mesh;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GeneratePlane(primitive.GetPlane()));
            mesh->Initialize();
            return mesh;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GenerateSphere(primitive.GetSphere()));
            mesh->Initialize();
            return mesh;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            auto mesh = std::make_unique<GLMesh>(MeshGenerator::GenerateTorus(primitive.GetTorus()));
            mesh->Initialize();
            return mesh;
        }
        default:
            return nullptr;
    }
}

} // namespace zero::render