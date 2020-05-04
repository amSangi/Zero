#pragma once

#include <memory>
#include <random>
#include <vector>
#include "GLMesh.hpp"
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
         * @param filename the filename associated with the 3D model.
         * @param rng the random number generator used to generate identifiers for child models
         * @param node the aiNode the model will be associated with
         * @param scene the aiScene the node is associated with
         * @return a GLModel instance
         */
        static std::shared_ptr<GLModel> CreateGLModel(const std::string& filename,
                                                      std::minstd_rand0 rng,
                                                      uint32 identifier,
                                                      const aiNode* node,
                                                      const aiScene* scene);

        /**
         * @brief Constructor. GLModels should be created through CreateGLModel.
         * @param meshes the different meshes that make up the model
         * @param transform the transform component prototype
         * @param material the material component prototype
         * @param volume the volume component prototype
         * @param model_instance the model instance component prototype
         */
        GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                Transform transform,
                Material material,
                Volume volume,
                ModelInstance mesh_instance);

        ~GLModel() override = default;

        /**
         * @see IModel::Draw
         */
        void Draw() override;

        /**
         * @see IModel::GetTransform
         */
        [[nodiscard]] Transform GetTransform() const override;

        /**
         * @see IModel::GetMaterial
         */
        [[nodiscard]] Material GetMaterial() const override;

        /**
         * @see IModel::GetVolume
         */
        [[nodiscard]] Volume GetVolume() const override;

        /**
         * @see IModel::GetModelInstance
         */
        [[nodiscard]] ModelInstance GetModelInstance() const override;

        /**
         * @see IModel::GetParent
         */
        [[nodiscard]] std::shared_ptr<IModel> GetParent() const override;

        /**
         * @see IModel::FindChild
         */
        [[nodiscard]] std::shared_ptr<IModel> FindChild(uint32 identifier) const override;

        /**
         * @see IModel::GetChildren
         */
        [[nodiscard]] const std::vector<std::shared_ptr<IModel>>& GetChildren() const override;

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
        std::shared_ptr<IModel> parent_model_;

        /**
         * @brief The child models
         */
        std::vector<std::shared_ptr<IModel>> child_models_;

        /**
         * @brief Transform prototype associated with the model
         */
        Transform transform_;

        /**
         * @brief Material prototype associated with this model
         */
        Material material_;

        /**
         * @brief Volume prototype encapsulating the model
         */
        Volume volume_;

        /**
         * @brief ModelInstance prototype for describing the entire model
         */
        ModelInstance model_instance_;

    }; // class GLModel

} // namespace zero::render