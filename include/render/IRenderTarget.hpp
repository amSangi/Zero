#pragma once

namespace zero::render
{

    class IRenderTarget
    {
    public:
        enum class Format
        {
            COLOR,
            DEPTH,
        };
        virtual ~IRenderTarget() = default;
        virtual void Initialize(Format format, uint32 width, uint32 height) = 0;
    }; // interface IRenderTarget

} // namespace zero::render