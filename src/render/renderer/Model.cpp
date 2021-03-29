#include "render/Model.hpp"
#include "math/Box.hpp"

namespace zero::render
{

Model::Model(Transform transform, Material material, Volume volume, ModelInstance model_instance)
: parent_model_(nullptr)
, child_models_()
, transform_(std::move(transform))
, material_(std::move(material))
, volume_(volume)
, model_instance_(std::move(model_instance))
{
}

zero::Transform Model::GetTransform() const
{
    return transform_;
}

Material Model::GetMaterial() const
{
    return material_;
}

Volume Model::GetVolume() const
{
    return volume_;
}

ModelInstance Model::GetModelInstance() const
{
    return model_instance_;
}

std::shared_ptr<Model> Model::GetParent() const
{
    return parent_model_;
}

void Model::SetParent(std::shared_ptr<Model> parent_model)
{
    parent_model_ = std::move(parent_model);
}

std::shared_ptr<Model> Model::FindChild(const std::string& node_name) const
{
    // Recursive breadth first search for child node
    for (const auto& model : child_models_)
    {
        if (model->GetModelInstance().node_name_ == node_name)
        {
            return model;
        }
    }
    for (const auto& model : child_models_)
    {
        auto search = model->FindChild(node_name);
        if (search)
        {
            return search;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<Model>>& Model::GetChildren() const
{
    return child_models_;
}

void Model::AddChild(std::shared_ptr<Model> child_model)
{
    child_models_.push_back(std::move(child_model));
}


} // namespace zero::render

