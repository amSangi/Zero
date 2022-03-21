#include "render/renderer/RenderQueue.hpp"

namespace zero::render
{

RenderQueue::RenderQueue(Sorter sorter)
: render_calls_()
, sorter_(std::move(sorter))
{
}

void RenderQueue::Submit(std::unique_ptr<IRenderCall> render_call)
{
    if (render_call)
    {
        render_calls_.push_back(std::move(render_call));
    }
}

void RenderQueue::Sort()
{
    std::sort(render_calls_.begin(), render_calls_.end(), sorter_);
}

void RenderQueue::ChangeSorter(Sorter sorter)
{
    sorter_ = std::move(sorter);
}

} // namespace zero::render