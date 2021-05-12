#pragma once

#include <string>
#include <memory>
#include "component/PrimitiveInstance.hpp"
#include "core/NonCopyable.hpp"
#include "render/IMeshLoader.hpp"

namespace zero::render
{

    class IModelManager : public IMeshLoader, public NonCopyable
    {
    public:
        virtual ~IModelManager() override = default;
        virtual void Initialize() = 0;
        virtual std::unique_ptr<IMesh> LoadMesh(std::unique_ptr<Mesh> mesh_data) override = 0;
        virtual void ClearModels() = 0;
        virtual void ClearPrimitives() = 0;
        virtual void AddModel(const std::string& model_name, std::shared_ptr<Model> model) = 0;
        virtual std::shared_ptr<Model> GetModel(const std::string& model_name) = 0;
        virtual std::shared_ptr<IMesh> GetPrimitive(const PrimitiveInstance& primitive_instance) = 0;
    };

} // namespace zero