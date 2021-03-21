#pragma once

#include <string>
#include "render/renderer/IUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLBaseUniformBuffer : public IUniformBuffer
    {
    public:
        GLBaseUniformBuffer(const std::string& name);
        ~GLBaseUniformBuffer() override;

        uint32 GetBindingIndex() const;
        const std::string& GetName() const;

    protected:
        void InitializeBaseBuffer(uint32 binding_index, uint32 data_size);

        std::string name_;
        GLuint buffer_id_;
        uint32 binding_index_;
        bool initialized_;
    };

} // namespace zero::render