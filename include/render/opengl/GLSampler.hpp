#pragma once

#include <GL/glew.h>
#include "render/ISampler.hpp"

namespace zero::render {

    class GLSampler : public ISampler {
    public:

        GLSampler();

        ~GLSampler() override;

        void SetWrappingS(Wrapping s) override;

        void SetWrappingT(Wrapping t) override;

        void SetMinificationFilter(Filter filter) override;

        void SetMagnificationFilter(Filter filter) override;

        void SetBorderColour(math::Vec4f colour) override;

        GLuint GetNativeIdentifier() const;

    protected:
        void Cleanup() override;

    private:

        /**
         * @brief The OpenGL sampler id
         */
        GLuint id_;

    }; // class GLSampler

} // namespace zero::render