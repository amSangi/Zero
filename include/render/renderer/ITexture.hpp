#pragma once

#include "core/NonCopyable.hpp"

namespace zero::render
{

    class ITexture : public NonCopyable
    {
    public:
        ITexture() = default;
        virtual ~ITexture() = default;
    }; // interface ITexture

} // namespace zero