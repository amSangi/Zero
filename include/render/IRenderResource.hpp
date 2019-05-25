#pragma once

#include "core/NonCopyable.hpp"

namespace zero::render {

    /**
     * @brief Root base class for render resources
     */
    class IRenderResource : private NonCopyable {
    public:

        /**
         * @brief Virtual destructor
         */
        virtual ~IRenderResource() = 0;

    protected:

        /**
         * @brief Cleanup render resources
         */
        virtual void Cleanup() = 0;

    }; // class IRenderResource

} // namespace zero::render