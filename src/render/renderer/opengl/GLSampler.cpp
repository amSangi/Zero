#include "render/renderer/opengl/GLSampler.hpp"

namespace zero::render
{

GLSampler::GLSampler(const GLuint sampler_id)
: id_(sampler_id)
{
}

void GLSampler::SetWrappingS(const Wrapping wrapping)
{
    switch (wrapping)
    {
        case Wrapping::CLAMP_TO_EDGE:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        case Wrapping::CLAMP_TO_BORDER:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            break;
        case Wrapping::MIRRORED_REPEAT:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        default:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
    }
}

void GLSampler::SetWrappingT(const Wrapping wrapping)
{
    switch (wrapping)
    {
        case Wrapping::CLAMP_TO_EDGE:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case Wrapping::CLAMP_TO_BORDER:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
        case Wrapping::MIRRORED_REPEAT:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        default:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
    }
}

void GLSampler::SetWrappingR(const Wrapping wrapping)
{
    switch (wrapping)
    {
        case Wrapping::CLAMP_TO_EDGE:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            break;
        case Wrapping::CLAMP_TO_BORDER:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
            break;
        case Wrapping::MIRRORED_REPEAT:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
            break;
        default:
            glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, GL_REPEAT);
            break;
    }
}

void GLSampler::SetMinificationFilter(const Filter filter)
{
    switch (filter)
    {
        case Filter::LINEAR:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case Filter::NEAREST_MIPMAP_NEAREST:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case Filter::LINEAR_MIPMAP_NEAREST:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case Filter::NEAREST_MIPMAP_LINEAR:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case Filter::LINEAR_MIPMAP_LINEAR:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default:
            glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
    }
}

void GLSampler::SetMagnificationFilter(const Filter filter)
{
    switch (filter)
    {
        case Filter::LINEAR:
            glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        default:
            glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
    }
}

void GLSampler::SetBorderColour(zero::math::Vec4f colour)
{
    glSamplerParameterfv(id_, GL_TEXTURE_BORDER_COLOR , colour.Data());
}

GLuint GLSampler::GetIdentifier() const
{
    return id_;
}

} // namespace zero::render