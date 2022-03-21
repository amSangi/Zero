#pragma once

#include "core/NonCopyable.hpp"

namespace zero::render
{

    class IFrameBuffer : public NonCopyable
    {
    public:
        IFrameBuffer() = default;
        virtual ~IFrameBuffer() = default;
    }; // class IFrameBuffer

} // namespace zero::render