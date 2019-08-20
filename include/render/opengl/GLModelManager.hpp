#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace zero::render {

    // Forward Declaration
    class GLModel;

    /**
     * @brief Manage
     */
    class GLModelManager {
    public:

        /**
         * @brief Default constructor
         */
        GLModelManager() = default;

        /**
         * @brief Default virtual destructor
         */
        virtual ~GLModelManager() = default;

        /**
         * @brief Load a model from a 3D file
         * @param filename the fully qualified filename of the 3D model
         * @return true if the model was loaded successfully. Otherwise false.
         */
        bool LoadModel(const std::string& filename);

        /**
         * @brief Clear all the models
         */
        void ClearModels();

        /**
         * @brief Get the GLModel associated with the given filename
         * @param filename the filename
         * @return the GLModel. Nullptr if the model does not exist.
         */
        std::shared_ptr<GLModel> GetModel(const std::string& filename);

    private:

        /**
         * @brief Filename to GLModel map
         */
        std::unordered_map<std::string, std::shared_ptr<GLModel>> models_;

    }; // class GLModelManager

} // namespace zero::render