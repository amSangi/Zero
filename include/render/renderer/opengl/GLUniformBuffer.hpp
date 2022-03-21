#pragma once

#include <string>
#include "render/renderer/IUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL graphics uniform buffer wrapper
     */
    class GLUniformBuffer final : public IUniformBuffer
    {
    public:
        GLUniformBuffer(GLuint ubo_id, uint32 buffer_size, std::string name);

        ~GLUniformBuffer() override = default;

        [[nodiscard]] uint32 GetSize() override;
        const std::string& GetName() override;
        [[nodiscard]] GLuint GetIdentifier() const;

    private:
        GLuint ubo_id_;
        uint32 buffer_size_;
        std::string name_;

    }; // class GLUniformBuffer

} // namespace zero::render