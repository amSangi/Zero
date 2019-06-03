#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL_opengl.h>
#include "GLShader.hpp"
#include "render/IProgram.hpp"

namespace zero::render {

    class GLProgram : public IProgram {
        template<class T>
        using UniformMap = std::unordered_map<std::string, T> ;
    public:

        static std::shared_ptr<GLProgram> CreateGLProgram(const std::vector<std::shared_ptr<GLShader>>& shaders);

        GLProgram();

        ~GLProgram() override;

        bool Link() override;

        bool IsLinked() const override;

        void Use() override;

        void Finish() override;

        void SetUniform(const std::string& name, math::Matrix4x4 value) override;

        void SetUniform(const std::string& name, math::Matrix3x3 value) override;

        void SetUniform(const std::string& name, math::Vec4f value) override;

        void SetUniform(const std::string& name, math::Vec3f value) override;

        void SetUniform(const std::string& name, uint32 value) override;

        void SetUniform(const std::string& name, int32 value) override;

        void SetUniform(const std::string& name, float value) override;

        GLuint GetNativeIdentifier();

    protected:
        void Cleanup() override;

    private:
        GLuint identifier_;
        UniformMap<math::Matrix4x4> matrix4x4_map_;
        UniformMap<math::Matrix3x3> matrix3x3_map_;
        UniformMap<math::Vec4f> vec4f_map_;
        UniformMap<math::Vec3f> vec3f_map_;
        UniformMap<uint32> uint32_map_;
        UniformMap<int32> int32_map_;
        UniformMap<float> float_map_;

    }; // class GLProgram

} // namespace zero::render