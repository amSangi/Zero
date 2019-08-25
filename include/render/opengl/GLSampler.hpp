#pragma once

#include "OpenGL.hpp"
#include "render/ISampler.hpp"

namespace zero::render {

    /**
     * @brief OpenGL texture sampler wrapper
     */
    class GLSampler : public ISampler {
    public:

        GLSampler();

        ~GLSampler() override;

        /**
         * @see ISampler::SetWrappingS
         */
        void SetWrappingS(Wrapping s) override;

        /**
         * @see ISampler::SetWrappingT
         */
        void SetWrappingT(Wrapping t) override;

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

    protected:
        /**
         * @brief Destroy the sampler resources
         */
        void Cleanup() override;

    private:

        /**
         * @brief The OpenGL sampler id
         */
        GLuint id_;

    }; // class GLSampler

} // namespace zero::render