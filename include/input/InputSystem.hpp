#pragma once

#include <memory>
#include <vector>
#include "core/ZBase.hpp"
#include "core/System.hpp"
#include "InputDevice.hpp"

namespace zero {
namespace input {

    /**
     * @brief The Input System manages all input devices and posts events
     * to the Engine's event bus
     */
    class InputSystem : public System {
        using DeviceContainer = std::vector<InputDevice*>;
        using Iterator = DeviceContainer::iterator;
        using ConstIterator = DeviceContainer::const_iterator;
    public:

        /**
         * @brief InputSystem Constructor
         * @param engine the engine this input system belongs to
         */
        explicit InputSystem(std::shared_ptr<Engine> engine)
        : System(std::move(engine)) {}

        /**
         * @brief Virtual Destructor
         */
        ~InputSystem() override = default;


        /* ********** Core System Overrides ********** */

        /**
         * @brief Initialize the InputSystem and its devices
         */
        void Initialize() override;

        /**
         * @brief Update the InputSystem
         * @param dt time between the current and last frame
         */
        void Update(float dt) override;

        /**
         * @brief Shutdown the InputSystem
         */
        void ShutDown() override;


        /* ********** Accessors ********** */

        /**
         * @brief Get a device by its unique identifier
         * @param device_id The unique device identifier
         * @return The InputDevice corresponding to the given id
         */
        const InputDevice* GetDevice(uint32 device_id) const;

        /**
         * @brief Get the iterator to the first element of the DeviceContainer.
         * If the container is empty, iterator will be equal to end().
         * @return an iterator to the first device
         */
        Iterator begin()            { return devices_.begin(); }

        /**
         * @brief Get the iterator to the element following the last device
         * @return an iterator following the last device
         */
        Iterator end()              { return devices_.end();   }

        /**
         * @brief Get the const iterator to the first element of the DeviceContainer.
         * If the container is empty, the iterator will be equal to cend().
         * @return a const iterator to the first device
         */
        ConstIterator begin() const { return devices_.begin(); }

        /**
         * @brief Get the const iterator to the element following the last device
         * @return a const iterator following the last device
         */
        ConstIterator end() const   { return devices_.end();   }

    protected:

        /**
         * @brief Container for the InputDevices
         */
        DeviceContainer devices_;

    }; // class InputSystem

} // namespace input
} // namespace zero