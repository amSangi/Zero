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
    }; // class AnimationSystem

} // namespace zero::animation