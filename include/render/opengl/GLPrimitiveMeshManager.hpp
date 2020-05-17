#pragma once

#include <memory>

namespace zero::render
{

    // Forward Declaration
    class GLMesh;
    class PrimitiveInstance;
    class Box;
    class Sphere;
    class Plane;
    class Cone;
    class Cylinder;
    class Torus;

    /**
     * @brief Manage the loading and lifetime of primitive meshes
     */
    class GLPrimitiveMeshManager
    {
    public:
        GLPrimitiveMeshManager();
        ~GLPrimitiveMeshManager() = default;

        /**
         * @brief Load the mesh of default primitives
         */
         void LoadPrimitives();
         /**
          * @brief Clear all primitive meshes
          */
         void ClearPrimitives();

         /**
          * @brief Get the mesh of a primitive
          * @param primitive_instance the primitive mesh data
          * @return a primitive mesh
          */
         [[nodiscard]] std::shared_ptr<GLMesh> GetPrimitiveMesh(PrimitiveInstance primitive_instance) const;

    private:
        /**
         * @brief Generate a new or return a cached primitive mesh
         *
         * If the mesh data is not too different from the default Shape, the cached mesh is returned.
         * Otherwise, a newly generated mesh is returned. See the equality operator for the different rendering
         * Shapes.
         */
        ///@{
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Box& box) const;
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Sphere& sphere) const;
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Plane& plane) const;
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Cone& cone) const;
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Cylinder& cylinder) const;
        [[nodiscard]] std::shared_ptr<GLMesh> Generate(const Torus& torus) const;
        ///@}

        /**
         * @brief Default primitive mesh cache
         */
        ///@{
        std::shared_ptr<GLMesh> box_mesh_;
        std::shared_ptr<GLMesh> sphere_mesh_;
        std::shared_ptr<GLMesh> plane_mesh_;
        std::shared_ptr<GLMesh> cone_mesh_;
        std::shared_ptr<GLMesh> cylinder_mesh_;
        std::shared_ptr<GLMesh> torus_mesh_;
        ///@}

    }; // class GLPrimitiveMeshManager

} // namespace zero::render
