#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "render/renderer/IProgram.hpp"
#include "render/renderer/opengl/OpenGL.hpp"
#include "render/renderer/opengl/GLShader.hpp"

namespace zero::render
{
    /**
     * @brief OpenGL graphics program wrapper
     */
    class GLProgram final : public IProgram
    {
        template<class T>
        using UniformMap = std::unordered_map<std::string, T> ;
    public:
        /**
         * @brief Constructor
         * @param program_id the OpenGL linked shader program identifier
         */
        explicit GLProgram(GLuint program_id);

        ~GLProgram() override = default;

        /**
         * @see IProgram::SetUniform
         */
        ///@{
        void SetUniform(const std::string& name, math::Matrix4x4 value) override;
        void SetUniform(const std::string& name, math::Matrix3x3 value) override;
        void SetUniform(const std::string& name, math::Vec4f value) override;
        void SetUniform(const std::string& name, math::Vec3f value) override;
        void SetUniform(const std::string& name, zero::int32 value) override;
        void SetUniform(const std::string& name, float value) override;
        ///@}

        const UniformMap<math::Matrix4x4>& GetMatrix4x4Map() const;
        const UniformMap<math::Matrix3x3>& GetMatrix3x3Map() const;
        const UniformMap<math::Vec4f>& GetVec4fMap() const;
        const UniformMap<math::Vec3f>& GetVec3fMap() const;
        const UniformMap<int32>& GetInt32Map() const;
        const UniformMap<float>& GetFloatMap() const;

        [[nodiscard]] GLuint GetIdentifier() const;

        /**
         * @brief Set all uniform data on the current program object
         */
        void FlushUniforms() const;
    private:
        GLuint program_id_;
        UniformMap<math::Matrix4x4> matrix4x4_map_;
        UniformMap<math::Matrix3x3> matrix3x3_map_;
        UniformMap<math::Vec4f> vec4f_map_;
        UniformMap<math::Vec3f> vec3f_map_;
        UniformMap<int32> int32_map_;
        UniformMap<float> float_map_;

    }; // class GLProgram

} // namespace zero::render