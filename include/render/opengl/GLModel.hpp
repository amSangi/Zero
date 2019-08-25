#pragma once

#include <memory>
#include <vector>
#include "OpenGL.hpp"
#include "render/IModel.hpp"
#include "math/Matrix4x4.hpp"

// Forward Declarations
class aiNode;
class aiScene;

namespace zero::render {

    /**
     * @brief OpenGL model wrapper
     */
    class GLModel : public IModel {
    public:

        /**
         * @brief Create a GLModel for an associated AssImporter aiNode and aiScene
         * @param node the aiNode the model will be associated with
         * @param scene the aiScene the node is associated with
         * @return a GLModel instance
         */
        static std::shared_ptr<GLModel> CreateGLModel(const aiNode* node, const aiScene* scene);

        /**
         * @brief Constructor. GLModels should be created through CreateGLModel.
         * @param vao the vertex array object identifier
         * @param vbo the interleaved vertex buffer object identifier
         * @param transformation the transformation of the model relative to the parent
         * @param material the material component prototype
         * @param volume the volume component prototype
         * @param parent the parent model
         * @param children the child models
         */
        GLModel(GLuint vao,
                GLuint vbo,
                math::Matrix4x4 transformation,
                Material material,
                Volume volume,
                std::shared_ptr<GLModel> parent,
                std::vector<std::shared_ptr<GLModel>> children);

        ~GLModel() override;

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
         * @brief Get the parent model
         * @return the parent GLModel. Nullptr if there is no parent.
         */
        [[nodiscard]] std::shared_ptr<GLModel> GetParent() const;

        /**
         * @brief Get the child models
         * @return the child GLModels. An empty vector if there are no children.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<GLModel>>& GetChildren() const;

    protected:
        /**
         * @brief Cleanup rendering resources
         */
        void Cleanup() override;

    private:

        /**
         * @brief Vertex Array Object identifier
         */
        GLuint vao_;

        /**
         * @brief Interleaved Vertex Buffer Identifier
         */
        GLuint vbo_;

        /**
         * @brief The Parent model
         */
        std::shared_ptr<GLModel> parent_model_;

        /**
         * @brief Child models
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

    }; // class GLModel

} // namespace zero::render