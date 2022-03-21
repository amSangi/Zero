#pragma once

#include <string>
#include "render/renderer/IFrameBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL graphics frame buffer wrapper
     */
    class GLFrameBuffer final : public IFrameBuffer
    {
    public:
        explicit GLFrameBuffer(GLuint fbo_id);

        ~GLFrameBuffer() override = default;

        [[nodiscard]] GLuint GetIdentifier() const;

    private:
        GLuint fbo_id_;

    }; // class GLFrameBuffer

} // namespace zero::render