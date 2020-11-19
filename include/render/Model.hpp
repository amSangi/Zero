#pragma once

#include "component/Transform.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/Volume.hpp"

namespace zero::render
{

    /**
     * @brief Abstract class for 3D models
     */
    class Model
    {
    public:

        /**
         * @brief Constructor
         * @param transform the transform component prototype
         * @param material the material component prototype
         * @param volume the volume component prototype
         * @param model_instance the model instance component prototype
         */
        Model(Transform transform, Material material, Volume volume, ModelInstance model_instance);

        virtual ~Model() = default;

        /**
         * @brief Draw the model on the current active rendering context
         */
        virtual void Draw() = 0;

        /**
         * @brief Get a transform associated with the model
         * @return a prototype of a transform component
         */
        [[nodiscard]] Transform GetTransform() const;

        /**
         * @brief Get a material associated with the model
         * @return a prototype of a material component
         */
        [[nodiscard]] Material GetMaterial() const;

        /**
         * @brief Get a volume associated with the model
         * @return a prototype of a volume component
         */
        [[nodiscard]] Volume GetVolume() const;

        /**
         * @brief Get a model instance associated with the model
         * @return a prototype of a model instance component
         */
        [[nodiscard]] ModelInstance GetModelInstance() const;

        /**
         * @brief Get the parent model
         * @return the parent Model. Nullptr if there is no parent.
         */
        [[nodiscard]] std::shared_ptr<Model> GetParent() const;

        /**
         * @brief Set the parent model
         * @param parent_model the parent model
         */
        void SetParent(std::shared_ptr<Model> parent_model);

        /**
         * @brief Find a child model with the given node name.
         * @param node_name the node name. This matches the node name from ModelInstance.
         * @return the child Model. Nullptr if no child exists with the given name.
         */
        [[nodiscard]] std::shared_ptr<Model> FindChild(const std::string& node_name) const;

        /**
         * @brief Get the child models
         * @return the child Model. An empty vector if there are no children.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<Model>>& GetChildren() const;

        /**
         * @brief Add a child model
         * @param child_model the child model
         */
        void AddChild(std::shared_ptr<Model> child_model);

    private:
        /**
         * @brief The parent model
         */
        std::shared_ptr<Model> parent_model_;

        /**
         * @brief The child models
         */
        std::vector<std::shared_ptr<Model>> child_models_;

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

    }; // abstract class Model

} // namespace zero::render