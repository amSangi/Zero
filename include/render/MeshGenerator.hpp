#pragma once

#include "Mesh.hpp"
#include "Shapes.hpp"
#include "math/Box.hpp"

namespace zero::render
{

    /**
     * @brief Static helper class to generate the mesh of primitive shapes
     */
    class MeshGenerator
    {
    public:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;

        /**
         * @brief Generate the mesh of a UV Sphere
         * @param sphere the sphere data
         * @return the mesh of a sphere
         */
        static Mesh GenerateSphere(const Sphere& sphere);

        /**
         * @brief Generate the mesh of a 2D plane
         * @param plane the plane data
         * @return the mesh of a plane
         */
        static Mesh GeneratePlane(const Plane& plane);

        /**
         * @brief Generate the mesh of a box with 24 vertices
         * @return the mesh of a 3D box
         */
        static Mesh GenerateBox();

        /**
         * @brief Generate the mesh of a torus
         * @param torus the torus data
         * @return the mesh of a torus
         */
        static Mesh GenerateTorus(const Torus& torus);

        /**
         * @brief Generate the mesh of a cone
         * @param cone the cone data
         * @return the mesh of a cone
         */
        static Mesh GenerateCone(const Cone& cone);

        /**
         * @brief Generate the mesh of a cylinder
         * @param cylinder the cylinder data
         * @return the mesh of a cylinder
         */
        static Mesh GenerateCylinder(const Cylinder& cylinder);

    private:
        /**
         * @brief Generate the mesh of a cylinder's cap
         * @param cylinder the cylinder data
         * @param index the context preserving vertex index
         * @param isTopCap is the cap the top cap of the cylinder?
         * @return the mesh of a cylinder cap
         */
        static Mesh GenerateCylinderCap(const Cylinder& cylinder, uint32& index, bool isTopCap);

    }; // class MeshGenerator

} // namespace zero::render
