#pragma once

#include "core/NonCopyable.hpp"
#include "render/Image.hpp"

namespace zero::render
{

    class ITexture : public NonCopyable
    {
    public:
        ITexture() = default;
        virtual ~ITexture() = default;
        virtual void Initialize(std::shared_ptr<Image> image) = 0;
    }; // interface ITexture

} // namespace zero