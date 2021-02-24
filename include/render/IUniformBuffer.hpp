#pragma once

#include "math/Matrix4x4.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Vector4.hpp"
#include "math/Vector3.hpp"

namespace zero::render
{

    class IUniformBuffer
    {
    public:
        virtual ~IUniformBuffer() = default;
        virtual void SetUniform(const std::string& name, math::Matrix4x4 value) = 0;
        virtual void SetUniform(const std::string& name, math::Matrix3x3 value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec4f value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec3f value) = 0;
        virtual void SetUniform(const std::string& name, zero::int32 value) = 0;
        virtual void SetUniform(const std::string& name, float value) = 0;
        virtual void FlushUniforms() = 0;
    }; // interface IUniformBuffer

} // namespace zero::render