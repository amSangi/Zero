#pragma once

#include <memory>

namespace zero
{
    // Forward declarations
    class PrimitiveInstance;

namespace render
{

    // Forward declarations
    class GLMesh;

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

} // namespace render
} // namespace zero
