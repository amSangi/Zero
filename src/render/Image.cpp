#include <SDL2/SDL_image.h>
#include "render/Image.hpp"

using namespace zero::render;

Image::Image(std::string filename)
: filename_(std::move(filename))
, surface_(nullptr)
{}

bool Image::Load() {
    if (surface_) return true;
    surface_ = IMG_Load(filename_.c_str());
    return surface_ != nullptr;
}

bool Image::Release() {
    if (!surface_) return false;
    SDL_FreeSurface(surface_);
    return true;
}

bool Image::IsLoaded() const {
    return surface_;
}

char* Image::GetData() const {
    return surface_ ? static_cast<char*>(surface_->pixels) : nullptr;
}

zero::uint32 Image::GetWidth() const {
    return surface_ ? surface_->w : 0;
}

zero::uint32 Image::GetHeight() const {
    return surface_ ? surface_->h : 0;
}

zero::uint32 Image::GetPitch() const {
    return surface_ ? surface_->pitch : 0;
}

zero::uint8 Image::BitsPerPixel() const {
    return surface_ ? surface_->format->BitsPerPixel : 0;
}

zero::uint8 Image::BytesPerPixel() const {
    return surface_ ? surface_->format->BytesPerPixel : 0;
}

Image::PixelFormat Image::GetPixelFormat() const {
    if (!surface_) return PixelFormat::PIXEL_FORMAT_UNDEFINED;

    switch (surface_->format->format)
    {
        case SDL_PIXELFORMAT_RGB332:
        default:
            return PixelFormat ::PIXEL_FORMAT_UNDEFINED;
    }
}