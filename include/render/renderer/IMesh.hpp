#pragma once

#include "core/NonCopyable.hpp"

namespace zero::render
{

    class IMesh : public NonCopyable
    {
    public:
        IMesh() = default;
        virtual ~IMesh() = default;
    }; // interface IMesh

} // namespace zero::render