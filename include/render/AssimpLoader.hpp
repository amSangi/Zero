#pragma once

#include <string>
#include "render/IModelBuilder.hpp"


// Forward declarations
class aiMesh;
class aiNode;
class aiNodeAnim;
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
        explicit AssimpLoader(IModelBuilder* model_builder);
        ~AssimpLoader() = default;

        /**
         * @brief Load a 3D model
         * @param model_name the name of the model
         * @param file_path the fully qualified model file path
         */
        void LoadModel(const std::string& model_name, const std::string& file_path);

    private:
        bool IsValidScene(const aiScene* ai_scene) const;
        void LoadNodeMap(aiNode* ai_node);
        void LoadAnimationMap(const aiScene* ai_scene);

        void LoadModel(std::shared_ptr<Model> parent_model,
                       const std::string& model_name,
                       const aiScene* ai_scene,
                       const aiNode* ai_node,
                       const math::Matrix4x4& accumulated_transform);

        math::Matrix4x4 ExtractTransformation(const aiNode* ai_node) const;
        std::unique_ptr<Mesh> ExtractMesh(aiMesh* ai_mesh) const;
        Material ExtractMaterial(const aiScene* ai_scene, const aiNode* ai_node) const;
        Volume ExtractVolume(aiMesh* ai_mesh) const;
        ModelInstance ExtractModelInstance(const std::string& model_name, const aiNode* ai_node) const;

        uint32 GetImportFlags() const;

        IModelBuilder* model_builder_;
        std::unordered_map<std::string, aiNode*> ai_node_map_;
        std::unordered_map<std::string, aiNodeAnim*> ai_animation_map_;

    }; // class AssimpLoader

} // namespace zero::render