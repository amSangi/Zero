#pragma once

#include <string>
#include "core/NonCopyable.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Vector4.hpp"
#include "math/Vector3.hpp"

namespace zero::render
{

    /**
     * @brief A Graphics Program containing Shaders of different types
     */
    class IProgram : public NonCopyable
    {
    public:

        virtual ~IProgram() = default;

        /**
         * @brief Set a Uniform variable. The program needs to be in use.
         * @param name The variable name
         * @param value The variable value
         */
        ///@{
        virtual void SetUniform(const std::string& name, math::Matrix4x4 value) = 0;
        virtual void SetUniform(const std::string& name, math::Matrix3x3 value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec4f value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec3f value) = 0;
        virtual void SetUniform(const std::string& name, zero::int32 value) = 0;
        virtual void SetUniform(const std::string& name, float value) = 0;
        ///@}

    }; // class IProgram

} // namespace zero::render