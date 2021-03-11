#pragma once

#include "core/NonCopyable.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Vector4.hpp"
#include "math/Vector3.hpp"

namespace zero::render
{

    class IUniformBuffer : public NonCopyable
    {
    public:
        IUniformBuffer() = default;
        virtual ~IUniformBuffer() = default;
    }; // interface IUniformBuffer

} // namespace zero::render