#pragma once

namespace zero {

    /*!
     * @brief Inheriting from NonCopyable prevents copying instances of the derived class
     */
    class NonCopyable {
    public:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;

    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;

    }; // class NonCopyable

} // namespace zero