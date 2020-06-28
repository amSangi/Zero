#pragma once

#include <memory>
#include <random>
#include <string>
#include <unordered_map>

// Forward Declarations
class aiScene;
class aiNode;

namespace zero
{
    // Forward declarations
    class ModelInstance;

namespace render
{

    // Forward Declaration
    class IModel;

    /**
     * @brief Manage the loading and lifetime of GLModels
     */
    class GLModelManager
    {
    public:

        GLModelManager();

        ~GLModelManager() = default;

        /**
         * @brief Load a model from a file. A single model may contain multiple sub models.
         * @param model_name the name of the model
         * @param filename the fully qualified filename of the model
         * @return true if the model was loaded successfully. Otherwise false.
         */
        bool LoadModel(const std::string& model_name, const std::string& filename);

        /**
         * @brief Clear all the models
         */
        void ClearModels();

        /**
         * @brief Get the IModel associated with the given model name
         * @param model_name the name of the model
         * @return the IModel. Nullptr if the model does not exist.
         */
        std::shared_ptr<IModel> GetModel(const std::string& model_name);

        /**
         * @brief Retrieve the correct IModel from a ModelInstance.
         * @param model_instance the model instance of the IModel
         * @return the IModel associated with the ModelInstance. Nullptr if it does not exist.
         */
        std::shared_ptr<IModel> GetModel(const ModelInstance& model_instance);

    private:
        /**
         * @brief Model name to IModel map
         */
        std::unordered_map<std::string, std::shared_ptr<IModel>> model_map_;

        /**
         * @brief Random Number Generator used to create model identifiers
         */
        std::minstd_rand0 random_generator_;

    }; // class GLModelManager

} // namespace render
} // namespace zero