#pragma once

#include "Mesh.hpp"
#include "math/Sphere.hpp"
#include "math/Box.hpp"

namespace zero::render {

    class MeshGenerator {
    public:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;

        static void Generate(const math::Sphere& sphere,
                             std::vector<Vertex>& vertices,
                             std::vector<uint32>& indices);

        static void Generate(const math::Box& box,
                             std::vector<Vertex>& vertices,
                             std::vector<uint32>& indices);

    }; // class MeshGenerator

} // namespace zero::render
