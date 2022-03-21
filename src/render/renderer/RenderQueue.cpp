#include "render/renderer/RenderQueue.hpp"

namespace zero::render
{

RenderQueue::RenderQueue(RenderCallComparator comparator)
: render_calls_()
, comparator_(std::move(comparator))
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
    std::sort(render_calls_.begin(), render_calls_.end(), comparator_);
}

void RenderQueue::ChangeSorter(RenderCallComparator comparator)
{
    comparator_ = std::move(comparator);
}

void RenderQueue::Render(const std::shared_ptr<IRenderHardware>& rhi)
{
    for (const std::unique_ptr<IRenderCall>& render_call : render_calls_)
    {
        render_call->Render(rhi);
    }
}

void RenderQueue::Clear()
{
    render_calls_.clear();
}

} // namespace zero::render