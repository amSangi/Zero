#pragma once

#include <string>
#include <memory>
#include "render/Mesh.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    class IModelManager
    {
    public:

        IModelManager() = default;
        virtual ~IModelManager() = default;

        /**
         * @brief Create and manage a new model
         * @param model_name the name of the model
         * @param mesh the mesh data for the model
         * @param transform the Transform prototype for the model
         * @param material the Material prototype for the model
         * @param volume the Volume prototype for the model
         * @param model_instance the ModelInstance prototype for the model
         * @return the newly created Model
         */
        virtual std::shared_ptr<Model> CreateModel(const std::string& model_name,
                                                   const Mesh& meshes,
                                                   const Transform& transform,
                                                   const Material& material,
                                                   const Volume& volume,
                                                   const ModelInstance& model_instance) = 0;

        /**
         * @brief Clear all the models
         */
        virtual void ClearModels() = 0;

        /**
         * @brief Get the Model associated with the given model name
         * @param model_name the name of the model
         * @return the Model. Default Model (not renderable) if the model does not exist.
         */
        virtual std::shared_ptr<Model> GetModel(const std::string& model_name) = 0;

        /**
         * @brief Retrieve the correct Model from a ModelInstance.
         * @param model_instance the model instance of the Model
         * @return the Model associated with the ModelInstance. Default Model (not renderable) if the model does not exist.
         */
        virtual std::shared_ptr<Model> GetModel(const ModelInstance& model_instance) = 0;
    };

} // namespace zero