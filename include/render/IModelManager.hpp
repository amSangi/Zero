#pragma once

#include <string>
#include "Components.hpp"

namespace zero::render {

    /**
     * @brief
     */
    class IModelManager {
    public:

        IModelManager() = default;

        virtual ~IModelManager() = default;

        virtual Component::Entity CreateModelInstance(const std::string& model) = 0;

        virtual bool LoadModel(const std::string& model) = 0;

    }; // class IModelManager

} // namespace zero::render