#pragma once

#include <string>
#include "math/Matrix4x4.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Vector4.hpp"
#include "math/Vector3.hpp"
#include "IRenderResource.hpp"

namespace zero::render {

    /**
     * @brief A Graphics Program containing Shaders of different types
     */
    class IProgram : public IRenderResource{
    public:

        /**
         * @brief Virtual Destructor
         */
        ~IProgram() override = 0;

        /**
         * @brief Link the graphics program
         * @return True if the link was successful. Otherwise false.
         */
        virtual bool Link() = 0;

        /**
         * @brief Set the graphics program as active on the current rendering state
         * @return True if the program is active. Otherwise false.
         */
        virtual bool Use() = 0;

        /**
         * @brief Removes the graphics program from the current rendering state
         * @return True if the program is no longer active. Otherwise false.
         */
        virtual bool Finish() = 0;

        /**
         * @brief Set a Uniform variable
         * @param name The variable name
         * @param value The variable value
         */
        ///@{
        virtual void SetUniform(const std::string& name, math::Matrix4x4 value) = 0;
        virtual void SetUniform(const std::string& name, math::Matrix3x3 value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec4f value) = 0;
        virtual void SetUniform(const std::string& name, math::Vec3f value) = 0;
        virtual void SetUniform(const std::string& name, uint32 value) = 0;
        virtual void SetUniform(const std::string& name, int32 value) = 0;
        virtual void SetUniform(const std::string& name, float value) = 0;
        ///@}

    }; // class IProgram

} // namespace zero::render