#pragma once

#include <memory>
#include "math/Box.hpp"
#include "math/Plane.hpp"
#include "math/Sphere.hpp"
#include "GLMesh.hpp"

namespace zero::render {

    /**
     * @brief Generate the mesh data for primitives given their mathematical representations
     */
    class GLPrimitiveGenerator {
    public:
        GLPrimitiveGenerator() = delete;
        ~GLPrimitiveGenerator() = delete;

        /**
         * @brief Generate the mesh of a sphere
         *
         * The generated mesh is an approximation and the radius may not be completely accurate
         *
         * @param sphere the sphere
         * @return a UV sphere approximating the sphere
         */
        static std::unique_ptr<GLMesh> Generate(const math::Sphere& sphere);

        /**
         * @brief Generate the mesh of a box
         * @param box the box
         * @return a mesh with 2 triangles per face
         */
        static std::unique_ptr<GLMesh> Generate(const math::Box& box);

    }; // class GLPrimitiveGenerator

} // namespace zero::render
