#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/opengl/GLMesh.hpp"
#include "render/MeshGenerator.hpp"

using namespace zero::render;

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(PrimitiveInstance primitive) {
    switch (primitive.GetType()) {
        case PrimitiveInstance::Type::BOX:
            return std::make_unique<GLMesh>(MeshGenerator::GenerateBox());
        case PrimitiveInstance::Type::CONE:
            return std::make_unique<GLMesh>(MeshGenerator::GenerateCone(primitive.GetCone()));
        case PrimitiveInstance::Type::CYLINDER:
            return std::make_unique<GLMesh>(MeshGenerator::GenerateCylinder(primitive.GetCylinder()));
        case PrimitiveInstance::Type::PLANE:
            return std::make_unique<GLMesh>(MeshGenerator::GeneratePlane(primitive.GetPlane()));
        case PrimitiveInstance::Type::SPHERE:
            return std::make_unique<GLMesh>(MeshGenerator::GenerateSphere(primitive.GetSphere()));
        case PrimitiveInstance::Type::TORUS:
            return std::make_unique<GLMesh>(MeshGenerator::GenerateTorus(primitive.GetTorus()));
        default:
            return nullptr;
    }
}