#pragma once

#include "render/IRenderable.hpp"

namespace zero::render
{

    class Renderable : public IRenderable
    {
    public:
        Renderable(const Material& material,
                   const Transform& transform,
                   const Volume& volume,
                   std::unique_ptr<PrimitiveInstance> primitive_instance,
                   std::shared_ptr<std::vector<math::Matrix4x4>> bone_matrices);
        ~Renderable() override;
        const Material& GetMaterial() override;
        const Transform& GetTransform() override;
        const Volume& GetVolume() override;

        PrimitiveInstance* GetPrimitiveInstance() override;
        std::shared_ptr<std::vector<math::Matrix4x4>> GetBoneMatrices() override;
    private:
        Material material_;
        Transform transform_;
        Volume volume_;
        std::unique_ptr<PrimitiveInstance> primitive_instance_;
        std::shared_ptr<std::vector<math::Matrix4x4>> bone_matrices_;
    }; // class Renderable

} // namespace zero::render