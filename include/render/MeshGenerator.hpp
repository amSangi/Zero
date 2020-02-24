#pragma once

#include "Mesh.hpp"
#include "math/Sphere.hpp"
#include "math/Box.hpp"

namespace zero::render {

    /**
     * @brief Static helper class to generate the mesh of primitive shapes
     */
    class MeshGenerator {
    public:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;

        /**
         * @brief Generate the mesh of a UV-sphere
         * @param sphere the mathematical representation of the sphere
         * @param latitude_count the number of latitudinal sections
         * @param longitude_count the number of longitudinal sections
         */
        static Mesh GenerateSphere(const math::Sphere& sphere,
                                   uint32 latitude_count = 32,
                                   uint32 longitude_count = 32);

        static Mesh GeneratePlane(const math::Vec3f& origin,
                                  const math::Vec3f& u_axis = math::Vec3f::Right(),
                                  const math::Vec3f& v_axis = math::Vec3f::Forward(),
                                  uint32 row_count = 8,
                                  uint32 column_count = 8);

        /**
         * @brief Generate the mesh of a box
         * @param box the mathematical representation of the sphere
         */
        static Mesh GenerateBox(const math::Box& box);

        static Mesh GenerateTorus();

        static Mesh GenerateCone();

        static Mesh GenerateCylinder();

    }; // class MeshGenerator

} // namespace zero::render
