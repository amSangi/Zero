#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "component/Component.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"

namespace zero
{
    struct Pose
    {
        Pose() = default;
        std::unordered_map<std::string, math::Matrix4x4> transforms_;
    }; // struct Pose

    struct VectorKey
    {
        math::Vec3f vector_;
        double time_;
    }; // struct VectorKey

    struct QuaternionKey
    {
        math::Quaternion quaternion_;
        double time_;
    }; // struct QuaternionKey

    struct AnimationChannel
    {
        AnimationChannel() = default;
        size_t GetScalingIndexAtTime(double time) const;
        size_t GetRotationIndexAtTime(double time) const;
        size_t GetTranslationIndexAtTime(double time) const;

        std::string name_;
        std::vector<VectorKey> scalings_;
        std::vector<QuaternionKey> rotations_;
        std::vector<VectorKey> translations_;
    }; // struct AnimationChannel

    struct Animation
    {
        Animation();

        std::string name_;
        double tick_rate_;
        double duration_;
        std::unordered_map<std::string, std::shared_ptr<AnimationChannel>> channel_map_;
    }; // struct Animation

    struct Animated : public Component
    {
        Animated();
        ~Animated();

        Entity root_bone_entity_;
        std::vector<std::string> bone_names_;
        std::vector<Entity> bone_entities_;
    };

    struct Animator : public Component
    {
        Animator();
        ~Animator();

        bool IsAnimating();
        void StopAnimation();
        void StartAnimation(const std::string& animation);
        const std::string& GetActiveAnimationName() const;
        std::vector<std::string> GetAnimationNames() const;

        void SetPose(std::unique_ptr<Pose> pose);
        Pose* GetActivePose() const;

        void AddAnimation(const std::string& animation_name, std::shared_ptr<Animation> animation);
        std::shared_ptr<Animation> GetActiveAnimation() const;

        void SetRootBoneEntity(Entity entity);
        Entity GetRootBoneEntity() const;
    private:
        Entity root_bone_entity_;
        std::string active_animation_;
        std::shared_ptr<Pose> active_pose_;
        std::unordered_map<std::string, std::shared_ptr<Animation>> animation_map_;

    }; // struct Animator

    struct Bone : public Component
    {
        Bone();
        ~Bone() = default;

        std::string name_;
        math::Matrix4x4 offset_matrix_;
    }; // struct Bone

} // namespace zero