#pragma once

#include <memory>
#include <vector>
#include "core/NonCopyable.hpp"
#include "render/renderer/IRenderCall.hpp"

namespace zero::render
{
    class RenderQueue : public NonCopyable
    {
    public:
        using Sorter = std::function<bool(const std::unique_ptr<IRenderCall>&, const std::unique_ptr<IRenderCall>&)>;

        explicit RenderQueue(Sorter sorter);
        ~RenderQueue() = default;

        void Submit(std::unique_ptr<IRenderCall> render_call);
        void Sort();
        void ChangeSorter(Sorter sorter);

    private:
        std::vector<std::unique_ptr<IRenderCall>> render_calls_;
        Sorter sorter_;

    }; // class RenderQueue

} // namespace zero::render