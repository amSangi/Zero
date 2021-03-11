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
         * @brief Link the graphics program
         * @return True if the link was successful. Otherwise false.
         */
        virtual bool Link() = 0;

        /**
         * @brief Is the Program linked?
         * @return True if the program is linked. Otherwise false.
         */
        [[nodiscard]] virtual bool IsLinked() const = 0;

        /**
         * @brief Set the graphics program as active on the current rendering state
         */
        virtual void Use() = 0;

        /**
         * @brief Removes the graphics program from the current rendering state
         */
        virtual void Finish() = 0;

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

        /**
         * @brief Set a Uniform variable in the graphics program. The program needs to be in use.
         * The value is flushed automatically.
         * @param name The variable name
         * @param value The variable value
         */
        ///@{
        virtual void FlushUniform(const std::string& name, math::Matrix4x4 value) = 0;
        virtual void FlushUniform(const std::string& name, math::Matrix3x3 value) = 0;
        virtual void FlushUniform(const std::string& name, math::Vec4f value) = 0;
        virtual void FlushUniform(const std::string& name, math::Vec3f value) = 0;
        virtual void FlushUniform(const std::string& name, zero::int32 value) = 0;
        virtual void FlushUniform(const std::string& name, float value) = 0;
        ///@}

        /**
         * @brief Set the uniform variables in the graphics program
         *   The graphics program must be active.
         */
        virtual void FlushUniforms() = 0;

    }; // class IProgram

} // namespace zero::render