#pragma once

#include <string>
#include <string_view>
#include "core/ZBase.hpp"

namespace zero {
namespace input {

    class InputSystem;

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

        /**
         * @brief Construct the device for the given InputSystem
         * @param input_system the InputSystem this device belongs to
         */
        explicit InputDevice(InputSystem* input_system)
        : input_system_(input_system) {}

        /**
         * @brief Virtual Destructor
         */
        virtual ~InputDevice() = 0;

        /**
         * @brief Update the device
         */
        virtual void Update() = 0;

        /**
         * @brief Get the type of the device
         * @return the type
         */
        virtual DeviceType GetType() const = 0;

        /**
         * @brief Get the device name
         * @return the name
         */
        virtual std::string_view GetName() const = 0;

        /**
         * @brief Get the state of the device
         * @return the state
         */
        virtual DeviceState GetState() const = 0;

        /**
         * @brief Get the unique device identifier
         * @return the 16-bit identifier
         */
        virtual uint16 GetId() const = 0;

        /**
         * @brief Check if the device is available
         * @return True if the device is not Unavailable. False otherwise.
         */
        inline bool IsAvailable() const { return GetState() != DEVICE_STATE_UNAVAILABLE; }


    protected:

        /**
         * @brief The InputSystem this device belongs to
         */
        const InputSystem* input_system_;

        /**
         * @brief The name of the device
         */
        std::string name_;

        /**
         * @brief The state of the device
         */
        DeviceState state_{ DEVICE_STATE_UNAVAILABLE };

        /**
         * @brief The unique identifier of the device
         */
        uint16 id_{ 0 };

    }; // abstract class InputDevice

} // namespace input
} // namespace zero