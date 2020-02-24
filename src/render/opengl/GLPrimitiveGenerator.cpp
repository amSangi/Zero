#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/MeshGenerator.hpp"

using namespace zero::render;

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const math::Sphere& sphere) {
    return std::make_unique<GLMesh>(MeshGenerator::GenerateSphere(sphere));
}

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const math::Box& box) {
    return std::make_unique<GLMesh>(MeshGenerator::GenerateBox(box));
}