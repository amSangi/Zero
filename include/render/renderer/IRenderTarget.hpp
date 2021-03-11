#pragma once

#include <memory>
#include "core/NonCopyable.hpp"
#include "render/renderer/ITexture.hpp"

namespace zero::render
{

    class IRenderTarget : public NonCopyable
    {
    public:
        enum class Format
        {
            COLOR,
            DEPTH,
        };
        virtual ~IRenderTarget() = default;
        virtual void Initialize(Format format, uint32 width, uint32 height, std::shared_ptr<ITexture> texture) = 0;

    }; // interface IRenderTarget

} // namespace zero::render