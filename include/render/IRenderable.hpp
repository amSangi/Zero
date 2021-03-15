#pragma once

#include "component/Animator.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"

namespace zero
{

    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;
        virtual const Material& GetMaterial() = 0;
        virtual const Transform& GetTransform() = 0;
        virtual const Volume& GetVolume() = 0;

        virtual Animator* GetAnimator() = 0;
        virtual ModelInstance* GetModelInstance() = 0;
        virtual PrimitiveInstance* GetPrimitiveInstance() = 0;
    }; // interface IRenderable

} // namespace zero