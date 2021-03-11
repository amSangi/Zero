#pragma once

#include <unordered_map>
#include "render/renderer/IModelManager.hpp"
#include "render/renderer/opengl/GLMesh.hpp"
#include "render/renderer/opengl/GLModel.hpp"

namespace zero::render
{

    class GLModelManager final : public IModelManager
    {
    public:

        GLModelManager();
        ~GLModelManager() override;

        void Initialize() override;
        std::shared_ptr<Model> CreateModel(const std::string& model_name,
                                           std::unique_ptr<Mesh> mesh,
                                           const Transform& transform,
                                           const Material& material,
                                           const Volume& volume,
                                           const ModelInstance& model_instance) override;
        void ClearModels() override;
        void ClearPrimitives() override;
        std::shared_ptr<Model> GetModel(const std::string& model_name) override;
        std::shared_ptr<Model> GetModel(const ModelInstance& model_instance) override;
        std::shared_ptr<IMesh> GetPrimitive(const PrimitiveInstance& primitive_instance) override;
    private:
        /**
         * @brief Generate a new or return a cached primitive mesh
         *
         * If the primitive data is not too different from the default Shape, the cached mesh is returned.
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

        std::unordered_map<std::string, std::shared_ptr<GLModel>> model_map_;

        /**
         * @brief Default primitive data
         */
        ///@{
        std::shared_ptr<GLMesh> box_mesh_;
        std::shared_ptr<GLMesh> sphere_mesh_;
        std::shared_ptr<GLMesh> plane_mesh_;
        std::shared_ptr<GLMesh> cone_mesh_;
        std::shared_ptr<GLMesh> cylinder_mesh_;
        std::shared_ptr<GLMesh> torus_mesh_;
        static Box kDefaultBox;
        static Sphere kDefaultSphere;
        static Plane kDefaultPlane;
        static Cone kDefaultCone;
        static Cylinder kDefaultCylinder;
        static Torus kDefaultTorus;
        ///@}

    }; // class GLModelManager

} // namespace render::render