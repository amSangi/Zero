#include "render/Skeleton.hpp"

namespace zero::render
{

Bone::Bone(const std::string& name)
: name_(name)
, parent_bone_(nullptr)
, child_bones_()
, inverse_root_transform_(math::Matrix4x4::Identity())
, transform_(math::Matrix4x4::Identity())
, offset_(math::Matrix4x4::Identity())
{
}

Skeleton::Skeleton(std::shared_ptr<Bone> root_bone, std::vector<std::shared_ptr<Bone>> bones)
: root_bone_(std::move(root_bone))
, bones_(std::move(bones))
{
}

void Skeleton::ApplyPose(const Pose& pose)
{
    PropagatePose(pose, root_bone_, root_bone_->inverse_root_transform_, math::Matrix4x4::Identity());
}

const std::vector<std::shared_ptr<Bone>>& Skeleton::GetBones() const
{
    return bones_;
}

void Skeleton::PropagatePose(const Pose& pose,
                             std::shared_ptr<Bone> bone,
                             const math::Matrix4x4& inverse_root_transformation,
                             const math::Matrix4x4& parent_transformation)
{
    auto transform_search = pose.transforms_.find(bone->name_);
    math::Matrix4x4 transformation = math::Matrix4x4::Identity();
    if (transform_search != pose.transforms_.end())
    {
        transformation = transform_search->second;
    }

    math::Matrix4x4 global_transformation = parent_transformation * transformation;
    bone->transform_ = inverse_root_transformation * global_transformation * bone->offset_;

    for (std::shared_ptr<Bone> child_bone : bone->child_bones_)
    {
        PropagatePose(pose, child_bone, global_transformation);
    }
}

} // namespace zero::render

