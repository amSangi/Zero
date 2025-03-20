#pragma once

#include "core/KeyCode.hpp"
#include "core/MouseButton.hpp"
#include "core/ZeroBase.hpp"
#include "math/Vector2.hpp"

namespace zero
{
    /**
     * @brief Static APIs for querying the keyboard/mouse states.
     */
    class Input
    {
    public:
        Input() = delete;
        ~Input() = delete;

        /**
         * @brief Is the key pressed down?
         * @param key_code the key
         * @return True if the key is pressed down on the keyboard. Otherwise false.
         */
        [[nodiscard]] static bool IsKeyDown(KeyCode key_code);

        /**
         * @brief Is the key up (i.e. not pressed down)?
         * @param key_code the key
         * @return True if the key is not pressed down on the keyboard. Otherwise false.
         */
        [[nodiscard]] static bool IsKeyUp(KeyCode key_code);

        /**
         * @brief Is the mouse button down?
         * @param mouse_button the mouse button
         * @return True if the mouse button is pressed down. Otherwise false.
         */
        [[nodiscard]] static bool IsMouseButtonDown(MouseButton mouse_button);

        /**
         * @brief Is the mouse button up (i.e. not pressed down)?
         * @param mouse_button the mouse button
         * @return True if the mouse button is not pressed down. Otherwise false.
         */
        [[nodiscard]] static bool IsMouseButtonUp(MouseButton mouse_button);

        /**
         * @brief Get the mouse position relative to the focus window
         * @return the mouse position relative to the focus window
         */
        [[nodiscard]] static math::Vec2f GetMousePosition();

        /**
         * @brief Get the global mouse position relative to the global desktop
         * @return the mouse position relative to the desktop
         */
        [[nodiscard]] static math::Vec2f GetGlobalMousePosition();

    }; // class Input

} // namespace zero