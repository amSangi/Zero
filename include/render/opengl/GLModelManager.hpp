#pragma once

#include <unordered_map>
#include "render/IModelManager.hpp"

namespace zero::render
{
    /**
     * @brief Manage the loading and lifetime of GLModels
     */
    class GLModelManager final : public IModelManager
    {
    public:

        GLModelManager();
        ~GLModelManager() override = default;

        /**
         * @see IModelManager::CreateModel
         */
        std::shared_ptr<Model> CreateModel(const std::string& model_name,
                                           const std::vector<Mesh>& meshes,
                                           const Transform& transform,
                                           const Material& material,
                                           const Volume& volume,
                                           const ModelInstance& model_instance) override;

        /**
         * @see IModelManager::ClearModels
         */
        void ClearModels() override;

        /**
         * @see IModelManager::GetModel
         */
        std::shared_ptr<Model> GetModel(const std::string& model_name) override;

        /**
         * @see IModelManager::GetModel
         */
        std::shared_ptr<Model> GetModel(const ModelInstance& model_instance) override;

    private:
        /**
         * @brief Model name to Model map
         */
        std::unordered_map<std::string, std::shared_ptr<Model>> model_map_;

        /**
         * @brief Represents a model without a mesh
         */
        std::shared_ptr<Model> empty_model_;

    }; // class GLModelManager

} // namespace render::render