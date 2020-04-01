#pragma once

#include "Mesh.hpp"
#include "Shapes.hpp"
#include "math/Box.hpp"

namespace zero::render {

    /**
     * @brief Static helper class to generate the mesh of primitive shapes
     */
    class MeshGenerator {
    public:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;

        static Mesh GenerateSphere(const Sphere& sphere);

        static Mesh GeneratePlane(const Plane& plane);

        static Mesh GenerateBox();

        static Mesh GenerateTorus(const Torus& torus);

        static Mesh GenerateCone();

        static Mesh GenerateCylinder();

    }; // class MeshGenerator

} // namespace zero::render
