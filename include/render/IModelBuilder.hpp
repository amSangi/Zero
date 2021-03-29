#pragma once

#include "render/IMesh.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    class IModelBuilder
    {
    public:
        virtual ~IModelBuilder() = default;
        virtual std::shared_ptr<Model> BuildModel(const std::string& model_name,
                                                  std::unique_ptr<Mesh> mesh,
                                                  const Transform& transform,
                                                  const Material& material,
                                                  const Volume& volume,
                                                  const ModelInstance& model_instance) = 0;
    };

} // namespace zero::render