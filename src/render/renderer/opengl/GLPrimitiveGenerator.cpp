#include "render/renderer/opengl/GLPrimitiveGenerator.hpp"
#include "render/renderer/MeshGenerator.hpp"

namespace zero::render
{

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const PrimitiveInstance& primitive)
{
    auto gl_mesh = std::make_unique<GLMesh>();
    switch (primitive.GetType())
    {
        case PrimitiveInstance::Type::BOX:
        {
            gl_mesh->Initialize(MeshGenerator::GenerateBox());
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            gl_mesh->Initialize(MeshGenerator::GenerateCone(primitive.GetCone()));
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            gl_mesh->Initialize(MeshGenerator::GenerateCylinder(primitive.GetCylinder()));
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            gl_mesh->Initialize(MeshGenerator::GeneratePlane(primitive.GetPlane()));
            break;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            gl_mesh->Initialize(MeshGenerator::GenerateSphere(primitive.GetSphere()));
            break;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            gl_mesh->Initialize(MeshGenerator::GenerateTorus(primitive.GetTorus()));
            break;
        }
    }

    return std::move(gl_mesh);
}

} // namespace zero::render