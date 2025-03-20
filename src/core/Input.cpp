#include "core/Input.hpp"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

namespace zero
{

bool Input::IsKeyDown(KeyCode key_code)
{
    const bool* keyboard_state = SDL_GetKeyboardState(nullptr);
    return keyboard_state[static_cast<SDL_Scancode>(key_code)];
}

bool Input::IsKeyUp(KeyCode key_code)
{
    return !IsKeyDown(key_code);
}

bool Input::IsMouseButtonDown(MouseButton mouse_button)
{
    uint32 button_mask = SDL_GetMouseState(nullptr, nullptr);
    switch (mouse_button)
    {
        case MouseButton::LEFT:
            return button_mask & SDL_BUTTON_LMASK;
        case MouseButton::RIGHT:
            return button_mask & SDL_BUTTON_RMASK;
        case MouseButton::MIDDLE:
            return button_mask & SDL_BUTTON_MMASK;
        case MouseButton::EXTRA_BUTTON_1:
            return button_mask & SDL_BUTTON_X2MASK;
        case MouseButton::EXTRA_BUTTON_2:
            return button_mask & SDL_BUTTON_X1MASK;
        default:
            return false;
    }
}

bool Input::IsMouseButtonUp(MouseButton mouse_button)
{
    return !IsMouseButtonDown(mouse_button);
}

math::Vec2f Input::GetMousePosition()
{
    math::Vec2f position{};
    SDL_GetMouseState(&position.x_, &position.y_);
    return position;
}

math::Vec2f Input::GetGlobalMousePosition()
{
    math::Vec2f position{};
    SDL_GetGlobalMouseState(&position.x_, &position.y_);
    return position;
}

} // namespace zero

