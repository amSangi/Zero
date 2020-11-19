#pragma once

#include <string>
#include "render/IModelManager.hpp"


// Forward declarations
class aiMesh;
class aiNode;
class aiScene;

namespace zero::render
{

    /**
     * @brief Load 3D model files using Assimp and convert them into Zero types
     */
    class AssimpLoader
    {
    public:
        /**
         * @brief Constructor
         * @param model_manager the model manager that manages all 3D models
         */
        explicit AssimpLoader(IModelManager* model_manager);
        ~AssimpLoader() = default;

        /**
         * @brief Load a 3D model
         * @param model_name the name of the model
         * @param model_file_path the fully qualified model file path
         */
        void LoadModel(const std::string& model_name, const std::string& model_file_path);

    private:
        std::shared_ptr<Model> LoadModel(const std::string& model_name, const aiScene* scene, const aiNode* node);
        Mesh LoadMesh(aiMesh* mesh) const;


        IModelManager* model_manager_;

    }; // class AssimpLoader

} // namespace zero::render