#pragma once

#include "render/ISampler.hpp"
#include "render/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief OpenGL texture sampler wrapper
     */
    class GLSampler final : public ISampler
    {
    public:

        GLSampler();

        ~GLSampler() override;

        /**
         * @see ISampler::Initialize
         */
        void Initialize() override;

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
        [[nodiscard]] GLuint GetNativeIdentifier() const;

    private:

        /**
         * @brief Destroy the sampler resources
         */
        void Cleanup();

        /**
         * @brief The OpenGL sampler id
         */
        GLuint id_;

    }; // class GLSampler

} // namespace zero::render