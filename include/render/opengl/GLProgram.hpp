#pragma once

#include <SDL_opengl.h>
#include "../IProgram.hpp"

namespace zero::render {

    class GLProgram : public IProgram {
    public:

        ~GLProgram();

        bool Link() override;

        bool Use() override;

        bool Finish() override;

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

    }; // class GLProgram

} // namespace zero::render