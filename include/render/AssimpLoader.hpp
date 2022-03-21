#pragma once

#include <string>
#include "render/IMeshLoader.hpp"
#include "component/Material.hpp"
#include "component/Volume.hpp"


// Forward declarations
class aiBone;
class aiMaterial;
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
         * @param mesh_loader the MeshData loader
         */
        explicit AssimpLoader(IMeshLoader* mesh_loader);
        ~AssimpLoader() = default;

        /**
         * @brief Load a 3D model
         * @param model_name the name of the model
         * @param file_path the fully qualified model file path
         * @return the Model instance. Nullptr if the model failed to load
         */
        std::shared_ptr<Model> LoadModel(const std::string& model_name, const std::string& file_path);

    private:
        bool IsValidScene(const aiScene* ai_scene) const;
        void LoadBoneMap(const aiScene* ai_scene);

        std::unique_ptr<Mesh>     ExtractMesh(aiMesh* ai_mesh) const;
        Material                  ExtractMaterial(const aiMaterial* ai_material) const;
        Volume                    ExtractVolume(aiMesh* ai_mesh) const;

        [[nodiscard]] constexpr uint32 GetImportFlags() const;

        IMeshLoader* mesh_loader_;
        std::unordered_map<std::string, aiBone*> bone_map_;

    }; // class AssimpLoader

} // namespace zero::render