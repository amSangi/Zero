#pragma once

namespace zero::input {

    /**
     * @brief Virtual mouse buttons
     */
    enum class MouseButton {
        MB_LEFT,     ///< Left mouse button
        MB_RIGHT,    ///< Right mouse button
        MB_MIDDLE,   ///< Middle mouse button
        MB_XBUTTON1, ///< Extra mouse button 1
        MB_XBUTTON2, ///< Extra mouse button 2

        MB_COUNT,    ///< Number of buttons. Keep this last

    }; // enum class MouseButton

} // namespace zero::input