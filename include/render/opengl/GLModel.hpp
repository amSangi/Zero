#pragma once

#include <memory>
#include <vector>
#include "GLMesh.hpp"
#include "math/Matrix4x4.hpp"
#include "render/IModel.hpp"

// Forward Declarations
class aiNode;
class aiScene;
class aiMesh;

namespace zero::render {

    /**
     * @brief Model containing OpenGL objects
     */
    class GLModel final : public IModel {
    public:

        /**
         * @brief Create a GLModel for an associated AssImp aiNode and aiScene
         * @param filename the filename the model is associated with
         * @param node the aiNode the model will be associated with
         * @param scene the aiScene the node is associated with
         * @return a GLModel instance
         */
        static std::shared_ptr<GLModel> CreateGLModel(const std::string& filename,
                                                      const aiNode* node,
                                                      const aiScene* scene);

        /**
         * @brief Constructor. GLModels should be created through CreateGLModel.
         * @param meshes the different meshes that make up the model
         * @param transformation the transformation of the model relative to the parent
         * @param material the material component prototype
         * @param volume the volume component prototype
         * @param mesh_instance the mesh instance component prototype
         */
        GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                math::Matrix4x4 transformation,
                Material material,
                Volume volume,
                MeshInstance mesh_instance);

        ~GLModel() override = default;

        /**
         * @see IModel::GetTransformation
         */
        [[nodiscard]] math::Matrix4x4 GetTransformation() const override;

        /**
         * @see IModel::GetMaterial
         */
        [[nodiscard]] Material GetMaterial() const override;

        /**
         * @see IModel::GetVolume
         */
        [[nodiscard]] Volume GetVolume() const override;

        /**
         * @see IModel::GetMeshInstance
         */
        [[nodiscard]] MeshInstance GetMeshInstance() const override;

        /**
         * @brief Get the parent model
         * @return the parent GLModel. Nullptr if there is no parent.
         */
        [[nodiscard]] std::shared_ptr<GLModel> GetParent() const;

        /**
         * @brief Get the child models
         * @return the child GLModels. An empty vector if there are no children.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<GLModel>>& GetChildren() const;

        /**
         * @brief Get the meshes that make up the model
         * @return the meshes of the model
         */
        [[nodiscard]] const std::vector<std::shared_ptr<GLMesh>>& GetMeshes() const;

    private:

        /**
         * @brief The different meshes that make up the model
         */
        std::vector<std::shared_ptr<GLMesh>> meshes_;

        /**
         * @brief The parent model
         */
        std::shared_ptr<GLModel> parent_model_;

        /**
         * @brief The child models
         */
        std::vector<std::shared_ptr<GLModel>> child_models_;

        /**
         * @brief Transformation relative to the parent model
         */
        math::Matrix4x4 transformation_;

        /**
         * @brief Material prototype associated with this model
         */
        Material material_;

        /**
         * @brief Volume prototype encapsulating the model
         */
        Volume volume_;

        /**
         * @brief MeshInstance prototype for describing the entire model
         */
        MeshInstance mesh_instance_;

    }; // class GLModel

} // namespace zero::render