#pragma once

#include "component/Shapes.hpp"
#include "math/Box.hpp"
#include "render/MeshData.hpp"

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
        static std::unique_ptr<MeshData> GenerateSphere(const Sphere& sphere);

        /**
         * @brief Generate the mesh of a 2D plane
         * @param plane the plane data
         * @return the mesh of a plane
         */
        static std::unique_ptr<MeshData> GeneratePlane(const Plane& plane);

        /**
         * @brief Generate the mesh of a box with 24 vertices
         * @return the mesh of a 3D box
         */
        static std::unique_ptr<MeshData> GenerateBox();

        /**
         * @brief Generate the mesh of a torus
         * @param torus the torus data
         * @return the mesh of a torus
         */
        static std::unique_ptr<MeshData> GenerateTorus(const Torus& torus);

        /**
         * @brief Generate the mesh of a cone
         * @param cone the cone data
         * @return the mesh of a cone
         */
        static std::unique_ptr<MeshData> GenerateCone(const Cone& cone);

        /**
         * @brief Generate the mesh of a cylinder
         * @param cylinder the cylinder data
         * @return the mesh of a cylinder
         */
        static std::unique_ptr<MeshData> GenerateCylinder(const Cylinder& cylinder);

    private:
        /**
         * @brief Generate the mesh of a cylinder's cap
         * @param cylinder the cylinder data
         * @param index the context preserving vertex index
         * @param is_top_cap is the cap the top cap of the cylinder?
         * @return the mesh of a cylinder cap
         */
        static std::unique_ptr<MeshData> GenerateCylinderCap(const Cylinder& cylinder, uint32& index, bool is_top_cap);

    }; // class MeshGenerator

} // namespace zero::render
