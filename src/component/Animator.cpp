#include "component/Animator.hpp"
#include <algorithm>

namespace zero
{

Animator::Animator()
: animations_()
, current_animation_()
{
}

void Animator::Stop()
{
    current_animation_.clear();
}

bool Animator::Play(const std::string& animation)
{
    auto search = std::find(animations_.begin(), animations_.end(), animation);
    if (search == animations_.end())
    {
        return false;
    }
    current_animation_ = animation;
    return true;
}

bool Animator::IsPlaying() const
{
    return !current_animation_.empty();
}

const std::string& Animator::GetCurrentAnimation() const
{
    return current_animation_;
}

const std::vector<std::string>& Animator::GetAnimations() const
{
    return animations_;
}


} // namespace zero

