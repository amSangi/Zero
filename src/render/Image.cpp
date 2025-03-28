#include "render/Image.hpp"
#include <SDL3_image/SDL_image.h>

namespace zero::render
{

Image::Image(std::string filename)
: filename_(std::move(filename))
, surface_(nullptr)
{
}

Image::~Image()
{
    Release();
}

bool Image::Load()
{
    if (surface_)
    {
        return true;
    }
    surface_ = IMG_Load(filename_.c_str());
    return surface_ != nullptr;
}

bool Image::Release()
{
    if (!surface_)
    {
        return false;
    }
    SDL_DestroySurface(surface_);
    surface_ = nullptr;
    return true;
}

bool Image::IsLoaded() const
{
    return surface_;
}

char* Image::GetData() const
{
    return surface_ ? static_cast<char*>(surface_->pixels) : nullptr;
}

zero::uint32 Image::GetWidth() const
{
    return surface_ ? surface_->w : 0;
}

zero::uint32 Image::GetHeight() const
{
    return surface_ ? surface_->h : 0;
}

zero::uint32 Image::GetPitch() const
{
    return surface_ ? surface_->pitch : 0;
}

Image::PixelFormat Image::GetPixelFormat() const
{
    if (!surface_)
    {
        return PixelFormat::UNDEFINED;
    }

    switch (surface_->format)
    {
        case SDL_PIXELFORMAT_XBGR1555:
        case SDL_PIXELFORMAT_BGR565:
        case SDL_PIXELFORMAT_BGR24:
        case SDL_PIXELFORMAT_XBGR8888:
            return PixelFormat::BGR;
        case SDL_PIXELFORMAT_RGB332:
        case SDL_PIXELFORMAT_XRGB4444:
        case SDL_PIXELFORMAT_XRGB1555:
        case SDL_PIXELFORMAT_RGB565:
        case SDL_PIXELFORMAT_RGB24:
        case SDL_PIXELFORMAT_XRGB8888:
            return PixelFormat::RGB;
        case SDL_PIXELFORMAT_RGBA4444:
        case SDL_PIXELFORMAT_RGBA5551:
        case SDL_PIXELFORMAT_RGBA8888:
        case SDL_PIXELFORMAT_RGBA32:
            return PixelFormat::RGBA;
        case SDL_PIXELFORMAT_BGRA4444:
        case SDL_PIXELFORMAT_BGRA5551:
        case SDL_PIXELFORMAT_BGRA8888:
        case SDL_PIXELFORMAT_BGRA32:
            return PixelFormat::BGRA;
        default:
            return PixelFormat::UNDEFINED;
    }
}

} // namespace zero::render