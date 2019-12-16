#pragma once

#include <memory>
#include "render/Components.hpp"
#include "GLMesh.hpp"

// Forward Declarations
class aiMesh;
class aiMaterial;

namespace zero::render {

    /**
     * @brief Convert AssImp objects into Zero engine specific objects
     */
    class GLAssetImportWrapper {
    public:
        GLAssetImportWrapper() = delete;
        ~GLAssetImportWrapper() = delete;

        /**
         * @brief Convert a aiMesh to a GLMesh object
         * @param mesh the aiMesh
         * @return a GLMesh instance
         */
        static std::shared_ptr<GLMesh> LoadMesh(aiMesh* mesh);

        /**
         * @brief Convert an aiMaterial to a Material component
         * @param material the aiMaterial
         * @return a Material component
         */
        static Material LoadMaterial(aiMaterial* ai_material);

    }; // class GLAssetImportWrapper

} // namespace zero::render