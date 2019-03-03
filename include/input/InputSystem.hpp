#pragma once

#include <vector>
#include "core/ZBase.hpp"
#include "core/System.hpp"
#include "InputDevice.hpp"

namespace zero {
namespace input {

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


        /* ********** System Methods ********** */

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


        /* ********** Device Methods ********** */

        /**
         * @brief Discover all devices
         */
        void DiscoverDevices();

        /**
         * @brief Get a device by its unique identifier
         * @param device_id The unique device identifier
         * @return The InputDevice corresponding to the given id
         */
        const InputDevice* GetDevice(uint32 device_id) const;

        /* ********** Iterators ********** */
        Iterator begin()            { return devices_.begin(); }
        Iterator end()              { return devices_.end();   }
        ConstIterator begin() const { return devices_.begin(); }
        ConstIterator end() const   { return devices_.end();   }

    protected:

        /**
         * @brief Container for the InputDevices
         */
        DeviceContainer devices_;

    }; // class InputSystem

} // namespace input
} // namespace zero