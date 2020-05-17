#pragma once

#include "core/Transform.hpp"
#include "Components.hpp"

namespace zero::render
{

    /**
     * @brief Base class for 3D render models
     */
    class IModel
    {
    public:

        virtual ~IModel() = default;

        /**
         * @brief Draw the IModel on the active rendering context
         */
        virtual void Draw() = 0;

        /**
         * @brief Get a transform associated with the model
         * @return a prototype of a transform component
         */
        [[nodiscard]] virtual Transform GetTransform() const = 0;

        /**
         * @brief Get a material associated with the model
         * @return a prototype of a material component
         */
        [[nodiscard]] virtual Material GetMaterial() const = 0;

        /**
         * @brief Get a volume associated with the model
         * @return a prototype of a volume component
         */
        [[nodiscard]] virtual Volume GetVolume() const = 0;

        /**
         * @brief Get a model instance associated with the model
         * @return a prototype of a model instance component
         */
        [[nodiscard]] virtual ModelInstance GetModelInstance() const = 0;

        /**
         * @brief Get the parent model
         * @return the parent IModel. Nullptr if there is no parent.
         */
        [[nodiscard]] virtual std::shared_ptr<IModel> GetParent() const = 0;

        /**
         * @brief Find a child model with the given identifier.
         * @param identifier the identifier
         * @return the child IModel. Nullptr if no child exists with the identifier.
         */
        [[nodiscard]] virtual std::shared_ptr<IModel> FindChild(uint32 identifier) const = 0;

        /**
         * @brief Get the child models
         * @return the child IModel. An empty vector if there are no children.
         */
        [[nodiscard]] virtual const std::vector<std::shared_ptr<IModel>>& GetChildren() const = 0;

    }; // class IModel

} // namespace zero::render