#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/MeshGenerator.hpp"

using namespace zero::render;

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const math::Sphere& sphere) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    MeshGenerator::Generate(sphere, vertices, indices);
    return std::make_unique<GLMesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<GLMesh> GLPrimitiveGenerator::Generate(const math::Box& box) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    MeshGenerator::Generate(box, vertices, indices);
    return std::make_unique<GLMesh>(std::move(vertices), std::move(indices));
}