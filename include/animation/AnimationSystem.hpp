#pragma once

#include "component/Transform.hpp"
#include "core/System.hpp"

namespace zero::animation
{

    class AnimationSystem : public System
    {
    public:
        explicit AnimationSystem(EngineCore* engine_core);
        ~AnimationSystem() override = default;
        void Initialize() override;
        void PreUpdate() override;
        void Update(const TimeDelta& time_delta) override;
        void PostUpdate() override;
        void ShutDown() override;

    private:
//        void ApplyAnimation(Animator& animator, const TimeDelta& time_delta);
//        void UpdateBoneTransform(Pose* pose, const Bone& bone, Transform& bone_transform, const math::Matrix4x4& parent_transformation);
//        std::unique_ptr<Pose> EvaluatePose(std::shared_ptr<Animation> animation, const TimeDelta& time_delta);
//        math::Matrix4x4 CalculateTranslation(std::shared_ptr<AnimationChannel> animation_channel, float tick);
//        math::Matrix4x4 CalculateRotation(std::shared_ptr<AnimationChannel> animation_channel, float tick);
//        math::Matrix4x4 CalculateScale(std::shared_ptr<AnimationChannel> animation_channel, float tick);

    }; // class AnimationSystem

} // namespace zero::animation