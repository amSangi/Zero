#pragma once

#include "render/renderer/IUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLBaseUniformBuffer : public IUniformBuffer
    {
    public:
        GLBaseUniformBuffer();
        ~GLBaseUniformBuffer() override;

        uint32 GetBindingIndex() const;

    protected:
        template<class T>
        void InitializeBaseBuffer(uint32 binding_index)
        {
            binding_index_ = binding_index;
            glGenBuffers(1, &buffer_id_);
            glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_STATIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, binding_index_, buffer_id_);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            initialized_ = true;
        }

    private:
        GLuint buffer_id_;
        uint32 binding_index_;
        bool initialized_;
    };

} // namespace zero::render