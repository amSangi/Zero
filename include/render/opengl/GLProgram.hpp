#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "OpenGL.hpp"
#include "render/IProgram.hpp"

namespace zero::render {

    // Forward declarations
    class GLShader;

    /**
     * @brief OpenGL graphics program wrapper
     */
    class GLProgram final : public IProgram {
        template<class T>
        using UniformMap = std::unordered_map<std::string, T> ;
    public:

        /**
         * @brief Create a new GLProgram given a series of GLShaders
         * @param shaders the shaders to use for the graphics program
         * @return a graphics program. Nullptr if an error occured.
         */
        static std::shared_ptr<GLProgram> CreateGLProgram(const std::vector<std::shared_ptr<GLShader>>& shaders);

        /**
         * @brief Constructor
         * @param id the OpenGL identifier
         */
        explicit GLProgram(GLuint id);

        ~GLProgram() override;

        bool Link() override;

        /**
         * @see IProgram::IsLinked
         */
        bool IsLinked() const override;

        /**
         * @see IProgram::IsLinked
         */
        void Use() override;

        /**
         * @see IProgram::IsLinked
         */
        void Finish() override;

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

        /**
         * @see IProgram::FlushUniform
         */
        ///@{
        void FlushUniform(const std::string& name, math::Matrix4x4 value) override;
        void FlushUniform(const std::string& name, math::Matrix3x3 value) override;
        void FlushUniform(const std::string& name, math::Vec4f value) override;
        void FlushUniform(const std::string& name, math::Vec3f value) override;
        void FlushUniform(const std::string& name, zero::int32 value) override;
        void FlushUniform(const std::string& name, float value) override;
        ///@}

        /**
         * @see IProgram::IsLinked
         */
        void FlushUniforms() override;

        /**
         * @brief Get the attribute location of a given variable
         * @param name the variable name
         * @return the attribute location
         */
        [[nodiscard]] GLint GetAttribLocation(const std::string& name) const;

    protected:
        /**
         * @brief Destroy the program resources
         */
        void Cleanup() override;

    private:
        GLuint id_;
        UniformMap<math::Matrix4x4> matrix4x4_map_;
        UniformMap<math::Matrix3x3> matrix3x3_map_;
        UniformMap<math::Vec4f> vec4f_map_;
        UniformMap<math::Vec3f> vec3f_map_;
        UniformMap<int32> int32_map_;
        UniformMap<float> float_map_;

    }; // class GLProgram

} // namespace zero::render