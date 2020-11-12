#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "render/IProgram.hpp"
#include "render/opengl/OpenGL.hpp"

namespace zero::render
{

    // Forward declarations
    class GLShader;

    /**
     * @brief OpenGL graphics program wrapper
     */
    class GLProgram final : public IProgram
    {
        template<class T>
        using UniformMap = std::unordered_map<std::string, T> ;
    public:

        /**
         * @brief Create a new GLProgram given a series of GLShaders
         * @param shaders the shaders to use for the graphics program
         * @return a graphics program. Nullptr if an error occurred.
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

        /**
         * @brief Get the uniform block index of a named uniform block
         * @param uniform_name the uniform name
         * @return the block index
         */
        [[nodiscard]] GLuint GetUniformBlockIndex(const std::string& uniform_name) const;

        /**
         * @brief Bind the index of the shader program to the associated binding point
         * @param block_index the index in the shader program
         * @param block_binding the binding point to bind the index to
         */
        void BindBlockIndex(GLuint block_index, GLuint block_binding) const;

        GLuint GetId() const { return id_; }
    private:

        /**
         * @brief Destroy the program resources
         */
        void Cleanup() const;

        GLuint id_;
        UniformMap<math::Matrix4x4> matrix4x4_map_;
        UniformMap<math::Matrix3x3> matrix3x3_map_;
        UniformMap<math::Vec4f> vec4f_map_;
        UniformMap<math::Vec3f> vec3f_map_;
        UniformMap<int32> int32_map_;
        UniformMap<float> float_map_;

    }; // class GLProgram

} // namespace zero::render