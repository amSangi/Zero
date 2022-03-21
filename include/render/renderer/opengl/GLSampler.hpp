#pragma once

#include "render/renderer/ISampler.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL texture sampler wrapper
     */
    class GLSampler final : public ISampler
    {
    public:

        explicit GLSampler(GLuint sampler_id);

        ~GLSampler() override = default;

        /**
         * @see ISampler::SetWrappingS
         */
        void SetWrappingS(Wrapping wrapping) override;

        /**
         * @see ISampler::SetWrappingT
         */
        void SetWrappingT(Wrapping wrapping) override;

        /**
         * @see ISampler::SetWrappingR
         */
        void SetWrappingR(Wrapping wrapping) override;

        /**
         * @see ISampler::SetMinificationFilter
         */
        void SetMinificationFilter(Filter filter) override;

        /**
         * @see ISampler::SetMagnificationFilter
         */
        void SetMagnificationFilter(Filter filter) override;

        /**
         * @see ISampler::SetBorderColour
         */
        void SetBorderColour(math::Vec4f colour) override;

        /**
         * @brief Get the native OpenGL identifier
         * @return the identifier
         */
        [[nodiscard]] GLuint GetIdentifier() const;

    private:
        /**
         * @brief The OpenGL sampler id
         */
        GLuint id_;

    }; // class GLSampler

} // namespace zero::render