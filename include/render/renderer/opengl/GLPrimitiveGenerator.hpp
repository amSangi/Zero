#pragma once

#include <memory>
#include "component/PrimitiveInstance.hpp"
#include "render/renderer/opengl/GLMesh.hpp"

namespace zero::render
{

    /**
     * @brief Generate the mesh data for primitives given their mathematical representations
     */
    class GLPrimitiveGenerator
    {
    public:
        GLPrimitiveGenerator() = delete;
        ~GLPrimitiveGenerator() = delete;

        /**
         * @brief Generate the GLMesh of a primitive
         * @param primitive the primitive
         * @return the GLMesh
         */
        static std::unique_ptr<GLMesh> Generate(const PrimitiveInstance& primitive);
    }; // class GLPrimitiveGenerator

} // namespace zero::render
