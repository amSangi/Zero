#pragma once

#include <string>
#include <memory>
#include "component/PrimitiveInstance.hpp"
#include "core/NonCopyable.hpp"
#include "render/IMesh.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    class IModelManager : public NonCopyable
    {
    public:
        virtual ~IModelManager() = default;
        virtual void Initialize() = 0;
        virtual std::shared_ptr<Model> CreateModel(const std::string& model_name,
                                                   std::unique_ptr<IMesh> mesh,
                                                   const Transform& transform,
                                                   const Material& material,
                                                   const Volume& volume,
                                                   const ModelInstance& model_instance) = 0;
        virtual void ClearModels() = 0;
        virtual std::shared_ptr<Model> GetModel(const std::string& model_name) = 0;
        virtual std::shared_ptr<Model> GetModel(const ModelInstance& model_instance) = 0;
        virtual std::shared_ptr<IMesh> GetPrimitive(const PrimitiveInstance& primitive_instance) = 0;
    };

} // namespace zero