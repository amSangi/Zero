#include "animation/AnimationSystem.hpp"

namespace zero::animation
{

AnimationSystem::AnimationSystem(EngineCore* engine_core)
: System(engine_core)
{
}

void AnimationSystem::Initialize()
{
}

void AnimationSystem::PreUpdate()
{
}

void AnimationSystem::PostUpdate()
{
}

void AnimationSystem::ShutDown()
{
}

void AnimationSystem::Update(const TimeDelta& time_delta)
{
    entt::registry& registry = GetCore()->GetRegistry();

    auto animation_view = registry.view<Animator, Transform>();
    for (Entity entity : animation_view)
    {
        Animator& animator = animation_view.get<Animator>(entity);
        if (animator.IsAnimating())
        {
            ApplyAnimation(animator, time_delta);
        }
    }
}

void AnimationSystem::ApplyAnimation(Animator& animator, const TimeDelta& time_delta)
{
    entt::registry& registry = GetCore()->GetRegistry();

    // Compute new skeletal pose
    std::unique_ptr<Pose> pose = EvaluatePose(animator.GetActiveAnimation(), time_delta);

    // Apply pose to bone transforms
    // This updates all bone transforms
    // Transforms attached to bones will be updated during the transform propagation step
    auto bone_view = registry.view<Transform, Bone>();
    Entity root_bone_entity = animator.GetRootBoneEntity();
    Transform& root_bone_transform = bone_view.get<Transform>(root_bone_entity);
    const Bone& root_bone = bone_view.get<Bone>(root_bone_entity);
    UpdateBoneTransform(pose.get(), root_bone, root_bone_transform);
}

void AnimationSystem::UpdateBoneTransform(Pose* pose, const Bone& bone, Transform& bone_transform)
{
    // TODO: Validate bone transform update
    // - Transforms updated properly?
    // - Should transforms be set or scaled/rotated/translated

    // Apply scale, rotation, and translation to bone transform
    math::Matrix4x4 pose_transformation = pose->transforms_[bone.GetName()];
    bone_transform.Scale(pose_transformation.GetScale());
    bone_transform.Rotate(pose_transformation.GetRotation());
    bone_transform.Translate(pose_transformation.GetTranslation());

    // Update child bones
    auto bone_view = GetCore()->GetRegistry().view<Transform, Bone>();
    for (Entity child_entity : bone_transform.children_)
    {
        // Ignore non-bone entities
        if (!bone_view.contains(child_entity))
        {
            continue;
        }

        Transform& child_bone_transform = bone_view.get<Transform>(child_entity);
        Bone& child_bone = bone_view.get<Bone>(child_entity);
        UpdateBoneTransform(pose, child_bone, child_bone_transform);
    }
}

std::unique_ptr<Pose> AnimationSystem::EvaluatePose(std::shared_ptr<Animation> animation, const TimeDelta& time_delta)
{
    float tick = time_delta.frame_delta_time_ * animation->tick_rate_;
    tick = math::FMod(tick, animation->duration_);

    std::unique_ptr<Pose> pose = std::make_unique<Pose>();
    for (const auto& name_channel_pair : animation->channel_map_)
    {
        const std::string& channel_name = name_channel_pair.first;
        std::shared_ptr<AnimationChannel> animation_channel = name_channel_pair.second;

        math::Matrix4x4 translation = CalculateTranslation(animation_channel, tick);
        math::Matrix4x4 rotation = CalculateRotation(animation_channel, tick);
        math::Matrix4x4 scale = CalculateScale(animation_channel, tick);

        pose->transforms_.emplace(channel_name, translation * rotation * scale);
    }

    return std::move(pose);
}

math::Matrix4x4 AnimationSystem::CalculateTranslation(std::shared_ptr<AnimationChannel> animation_channel, float tick)
{
    math::Matrix4x4 identity = math::Matrix4x4::Identity();
    if (animation_channel->translations_.empty())
    {
        return identity;
    }
    if (animation_channel->translations_.size() == 1)
    {
        return identity.Translate(animation_channel->translations_.front().vector_);
    }

    // Get the previous and next translations given the tick
    size_t previous_index = animation_channel->GetTranslationIndexAtTime(tick);
    VectorKey previous_key = animation_channel->translations_[previous_index];
    VectorKey next_key = previous_key;
    if (previous_index + 1 < animation_channel->translations_.size())
    {
        next_key = animation_channel->translations_[previous_index + 1];
    }

    // Compute time between the two animation key frames
    float key_delta = next_key.time_ - previous_key.time_;
    // Compute the percentage of how far along the animation is between two key frames
    float animation_percentage = (tick - previous_key.time_) / key_delta;
    animation_percentage = math::Clamp(animation_percentage, 0.0F, 1.0F);

    math::Vec3f interpolated_translation = math::Vec3f::Lerp(previous_key.vector_, next_key.vector_, animation_percentage);
    return identity.Translate(interpolated_translation);
}

math::Matrix4x4 AnimationSystem::CalculateRotation(std::shared_ptr<AnimationChannel> animation_channel, float tick)
{
    math::Matrix4x4 identity = math::Matrix4x4::Identity();
    if (animation_channel->rotations_.empty())
    {
        return identity;
    }
    if (animation_channel->rotations_.size() == 1)
    {
        return identity.Rotate(animation_channel->rotations_.front().quaternion_);
    }

    // Get the previous and next rotations given the tick
    size_t previous_index = animation_channel->GetRotationIndexAtTime(tick);
    QuaternionKey previous_key = animation_channel->rotations_[previous_index];
    QuaternionKey next_key = previous_key;
    if (previous_index + 1 < animation_channel->rotations_.size())
    {
        next_key = animation_channel->rotations_[previous_index + 1];
    }

    // Compute time between the two animation key frames
    float key_delta = next_key.time_ - previous_key.time_;
    // Compute the percentage of how far along the animation is between two key frames
    float animation_percentage = (tick - previous_key.time_) / key_delta;
    animation_percentage = math::Clamp(animation_percentage, 0.0F, 1.0F);

    math::Quaternion interpolated_rotation = math::Quaternion::Slerp(previous_key.quaternion_, next_key.quaternion_, animation_percentage);
    return identity.Rotate(interpolated_rotation);
}

math::Matrix4x4 AnimationSystem::CalculateScale(std::shared_ptr<AnimationChannel> animation_channel, float tick)
{
    math::Matrix4x4 identity = math::Matrix4x4::Identity();
    if (animation_channel->scalings_.empty())
    {
        return identity;
    }
    if (animation_channel->scalings_.size() == 1)
    {
        return identity.Scale(animation_channel->scalings_.front().vector_);
    }

    // Get the previous and next scales given the tick
    size_t previous_index = animation_channel->GetScalingIndexAtTime(tick);
    VectorKey previous_key = animation_channel->scalings_[previous_index];
    VectorKey next_key = previous_key;
    if (previous_index + 1 < animation_channel->scalings_.size())
    {
        next_key = animation_channel->scalings_[previous_index + 1];
    }

    // Compute time between the two animation key frames
    float key_delta = next_key.time_ - previous_key.time_;
    // Compute the percentage of how far along the animation is between two key frames
    float animation_percentage = (tick - previous_key.time_) / key_delta;
    animation_percentage = math::Clamp(animation_percentage, 0.0F, 1.0F);

    math::Vec3f interpolated_scale = math::Vec3f::Lerp(previous_key.vector_, next_key.vector_, animation_percentage);
    return identity.Scale(interpolated_scale);
}

} // namespace zero::animation

