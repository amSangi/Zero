#include "component/Animator.hpp"

namespace zero
{
struct CompareAnimationKeys
{
    bool operator() (const VectorKey& left, const VectorKey& right)
    {
        return left.time_ < right.time_;
    }

    bool operator() (float left_time, const VectorKey& right)
    {
        return left_time < right.time_;
    }

    bool operator() (const QuaternionKey& left, const QuaternionKey& right)
    {
        return left.time_ < right.time_;
    }

    bool operator() (float left_time, const QuaternionKey& right)
    {
        return left_time < right.time_;
    }
};

size_t AnimationChannel::GetScalingIndexAtTime(double time) const
{
    auto iterator = std::upper_bound(scalings_.begin(), scalings_.end(), time, CompareAnimationKeys{});
    return iterator - scalings_.begin();
}

size_t AnimationChannel::GetRotationIndexAtTime(double time) const
{
    auto iterator = std::upper_bound(rotations_.begin(), rotations_.end(), time, CompareAnimationKeys{});
    return iterator - rotations_.begin();
}

size_t AnimationChannel::GetTranslationIndexAtTime(double time) const
{
    auto iterator = std::upper_bound(translations_.begin(), translations_.end(), time, CompareAnimationKeys{});
    return iterator - translations_.begin();
}

Animation::Animation()
: name_()
, tick_rate_(0.0F)
, duration_(0.0F)
, channel_map_()
{
}

Animated::Animated()
: root_bone_entity_(NullEntity)
, inverse_root_transform_(math::Matrix4x4::Identity())
, bone_names_()
, bone_entities_()
{
}

Animated::~Animated()
{
}

Animator::Animator()
: root_bone_entity_(NullEntity)
, active_animation_()
, active_pose_()
, animation_map_()
{
}


Animator::~Animator()
{
}

bool Animator::IsAnimating()
{
    return GetActiveAnimation() != nullptr;
}

void Animator::StopAnimation()
{
    active_animation_.clear();
}

void Animator::StartAnimation(const std::string& animation)
{
    active_animation_ = animation;
}

const std::string& Animator::GetActiveAnimationName() const
{
    return active_animation_;
}

std::vector<std::string> Animator::GetAnimationNames() const
{
    std::vector<std::string> animation_names{};
    animation_names.reserve(animation_map_.size());

    for (const auto& animation_name_pair : animation_map_)
    {
        const std::string& animation_name = animation_name_pair.first;
        animation_names.push_back(animation_name);
    }
    return animation_names;
}

void Animator::SetPose(std::unique_ptr<Pose> pose)
{
    active_pose_ = std::move(pose);
}

Pose* Animator::GetActivePose() const
{
    return active_pose_.get();
}

void Animator::AddAnimation(const std::string& animation_name, std::shared_ptr<Animation> animation)
{
    animation_map_.emplace(animation_name, animation);
}

std::shared_ptr<Animation> Animator::GetActiveAnimation() const
{
    auto animation_search = animation_map_.find(active_animation_);
    if (animation_search == animation_map_.end())
    {
        return nullptr;

    }
    return animation_search->second;
}

void Animator::SetRootBoneEntity(Entity entity)
{
    root_bone_entity_ = entity;
}

Entity Animator::GetRootBoneEntity() const
{
    return root_bone_entity_;
}


Bone::Bone()
: name_()
, offset_matrix_(math::Matrix4x4::Identity())
{
}

} // namespace zero

