#pragma once

#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <core/ZBase.hpp>

// Forward Declarations
class aiScene;
class aiNode;

namespace zero::render {

    // Forward Declaration
    class GLModel;

    /**
     * @brief Manage the loading and lifetime of GLModels
     */
    class GLModelManager {
    public:

        GLModelManager();

        virtual ~GLModelManager() = default;

        /**
         * @brief Load a model from a 3D file. A single 3D file may contain multiple sub models.
         * @param filename the fully qualified filename of the 3D model
         * @return true if the model was loaded successfully. Otherwise false.
         */
        bool LoadModel(const std::string& filename);

        /**
         * @brief Initialize all GLModels
         */
        void InitializeModels();

        /**
         * @brief Clear all the models
         */
        void ClearModels();

        /**
         * @brief Get the GLModel associated with the given filename
         * @param filename the fully qualified filename
         * @return the GLModel. Nullptr if the model does not exist.
         */
        std::shared_ptr<GLModel> GetModel(const std::string& filename);

    private:
        /**
         * @brief Filename to GLModel map
         */
        std::unordered_map<std::string, std::shared_ptr<GLModel>> model_map_;

        /**
         * @brief Random Number Generator used to create model identifiers
         */
        std::minstd_rand0 random_generator_;

    }; // class GLModelManager

} // namespace zero::render