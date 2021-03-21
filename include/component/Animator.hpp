#pragma once

#include "component/Component.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    struct Animator : public Component
    {
        Animator();
        ~Animator();
        // TODO: Create Animator component

    private:
        math::Vec3f dummy_data_;
    }; // class Animator

} // namespace zero