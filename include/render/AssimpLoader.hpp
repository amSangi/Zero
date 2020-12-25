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
         * @brief Load a Wavefront OBJ model
         * @param model_name the name of the model
         * @param obj_file_path the fully qualified model file path
         */
        void LoadOBJModel(const std::string& model_name, const std::string& obj_file_path);

        /**
         * @brief Load an FBX model
         * @param model_name the name of the model
         * @param fbx_file_path the fully qualified model file path
         */
        void LoadFBXModel(const std::string& model_name, const std::string& fbx_file_path);

    private:
        std::shared_ptr<Model> LoadModel(const std::string& model_name, const aiScene* scene, const aiNode* node);
        Mesh LoadMesh(aiMesh* mesh) const;

        uint32 GetOBJImportFlags() const;
        uint32 GetFBXImportFlags() const;

        IModelManager* model_manager_;

    }; // class AssimpLoader

} // namespace zero::render