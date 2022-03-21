#pragma once

#include <memory>
#include <vector>
#include "core/NonCopyable.hpp"
#include "render/renderer/IRenderCall.hpp"

namespace zero::render
{
    /**
     * @brief Manage, sort, and execute a list of render calls
     */
    class RenderQueue : public NonCopyable
    {
    public:
        /**
         * @brief The render call comparison function used for sorting
         * Returns true if the first argument is less than (i.e. is ordered before) the second.
         */
        using RenderCallComparator = std::function<bool(const std::unique_ptr<IRenderCall>&, const std::unique_ptr<IRenderCall>&)>;

        /**
         * @brief Constructor
         * @param comparator the render call comparison function to use for sorting
         */
        explicit RenderQueue(RenderCallComparator comparator);

        /**
         * @brief Default destructor
         */
        ~RenderQueue() = default;

        /**
         * @brief Submit a new render call for sorting and execution
         * @param render_call the new render call to manage
         */
        void Submit(std::unique_ptr<IRenderCall> render_call);

        /**
         * @brief Sort the render calls using the set render call sorter
         */
        void Sort();

        /**
         * @brief Update the render call comparison function
         * @param comparator the new comparison function to use
         */
        void ChangeSorter(RenderCallComparator comparator);

        /**
         * @brief Render the render calls
         * @param rhi the render hardware interface to use for rendering
         */
        void Render(const std::shared_ptr<IRenderHardware>& rhi);

        /**
         * @brief Clear the list of render calls
         */
        void Clear();

    private:
        /**
         * @brief The list of render calls
         */
        std::vector<std::unique_ptr<IRenderCall>> render_calls_;

        /**
         * @brief The render call comparison function
         */
        RenderCallComparator comparator_;

    }; // class RenderQueue

} // namespace zero::render