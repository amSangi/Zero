#pragma once

#include <memory>
#include "render/Components.hpp"

namespace zero::render {

    // Forward declarations
    class GLMesh;
    class PrimitiveInstance;

    /**
     * @brief Generate the mesh data for primitives given their mathematical representations
     */
    class GLPrimitiveGenerator {
    public:
        GLPrimitiveGenerator() = delete;
        ~GLPrimitiveGenerator() = delete;

        /**
         * @brief Generate the GLMesh of a primitive
         * @param primitive the primitive
         * @return the GLMesh
         */
        static std::unique_ptr<GLMesh> Generate(PrimitiveInstance primitive);
    }; // class GLPrimitiveGenerator

} // namespace zero::render
