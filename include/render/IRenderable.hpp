#pragma once

#include "component/Animator.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"

namespace zero::render
{

    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;
        virtual const Material& GetMaterial() = 0;
        virtual const Transform& GetTransform() = 0;
        virtual const Volume& GetVolume() = 0;

        virtual ModelInstance* GetModelInstance() = 0;
        virtual PrimitiveInstance* GetPrimitiveInstance() = 0;
        virtual std::shared_ptr<std::vector<math::Matrix4x4>> GetBoneMatrices() = 0;
    }; // interface IRenderable

} // namespace zero