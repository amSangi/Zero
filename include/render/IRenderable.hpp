#pragma once

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
        virtual Material* GetMaterial() = 0;
        virtual ModelInstance* GetModelInstance() = 0;
        virtual PrimitiveInstance* GetPrimitiveInstance() = 0;
        virtual Transform* GetTransform() = 0;
        virtual Volume* GetVolume() = 0;
    }; // interface IRenderable

} // namespace zero