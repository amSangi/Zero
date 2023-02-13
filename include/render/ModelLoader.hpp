#pragma once

#include <string>
#include "render/Model.hpp"

namespace zero::render
{
    /**
     * @brief Load 3D asset files as EntityPrototype instances
     */
    class ModelLoader
    {
    public:
        ModelLoader() = delete;

        /**
         * @brief Load a 3D model
         * @param model_name the name of the 3D model
         * @param file_path the fully qualified model file path
         * @return the Model instance. Nullptr if the model failed to load
         */
        static std::unique_ptr<Model> LoadModel(const std::string& model_name, const std::string& file_path);

    }; // class ModelLoader

} // namespace zero::render