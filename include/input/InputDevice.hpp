#pragma once

#include <string_view>
#include "core/ZBase.hpp"

namespace zero {
namespace input {

    /**
     * @brief The type of the device
     */
    enum DeviceType {
        DEVICE_TYPE_MOUSE,
        DEVICE_TYPE_KEYBOARD
    }; // enum DeviceType

    /**
     * @brief The state of the device
     */
    enum DeviceState {
        DEVICE_STATE_AVAILABLE,
        DEVICE_STATE_UNAVAILABLE,
    }; // enum DeviceState

    /**
     * @brief The value type for a given input
     */
    enum InputType {
        INPUT_TYPE_BOOL,            // True/False or 0/1 or Up/Down
        INPUT_TYPE_DECIMAL          // [-1, 1] or [0, 1]
    }; // enum InputType


    /**
     * @brief Abstract class representing all input devices
     */
    class InputDevice {
    public:
        virtual ~InputDevice() = 0;

        virtual void Update() = 0;

        /* ********** Accessors ********** */
        virtual DeviceState GetState() const = 0;
        virtual DeviceType GetType() const = 0;
        virtual std::string_view GetName() const = 0;
        virtual uint16 GetId() const = 0;
        virtual bool IsAvailable() { return GetState() != DEVICE_STATE_UNAVAILABLE; }

    }; // abstract class InputDevice

} // namespace input
} // namespace zero