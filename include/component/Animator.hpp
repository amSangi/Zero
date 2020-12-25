#pragma once

#include <memory>
#include <vector>
#include "component/Component.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero
{
    struct Animator : public Component
    {
        Animator();

        void Stop();
        bool Play(const std::string& animation);
        bool IsPlaying() const;
        const std::string& GetCurrentAnimation() const;
        const std::vector<std::string>& GetAnimations() const;

    private:
        std::vector<std::string> animations_;
        std::string current_animation_;

    }; // struct Animator

} // namespace zero