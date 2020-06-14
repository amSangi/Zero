#include <algorithm>
#include "core/EventBus.hpp"
#include <SDL_events.h>

namespace zero
{

KeyCode SDLKeyCodeToZeroKeyCode(SDL_Keycode sdl_key_code)
{
    switch (sdl_key_code)
    {
        //////////////////////////////////////////////////
        ///// Alphabet Keys
        //////////////////////////////////////////////////
        case SDLK_a:
            return KeyCode::A;
        case SDLK_b:
            return KeyCode::B;
        case SDLK_c:
            return KeyCode::C;
        case SDLK_d:
            return KeyCode::D;
        case SDLK_e:
            return KeyCode::E;
        case SDLK_f:
            return KeyCode::F;
        case SDLK_g:
            return KeyCode::G;
        case SDLK_h:
            return KeyCode::H;
        case SDLK_i:
            return KeyCode::I;
        case SDLK_j:
            return KeyCode::J;
        case SDLK_k:
            return KeyCode::K;
        case SDLK_l:
            return KeyCode::L;
        case SDLK_m:
            return KeyCode::M;
        case SDLK_n:
            return KeyCode::N;
        case SDLK_o:
            return KeyCode::O;
        case SDLK_p:
            return KeyCode::P;
        case SDLK_q:
            return KeyCode::Q;
        case SDLK_r:
            return KeyCode::R;
        case SDLK_s:
            return KeyCode::S;
        case SDLK_t:
            return KeyCode::T;
        case SDLK_u:
            return KeyCode::U;
        case SDLK_v:
            return KeyCode::V;
        case SDLK_w:
            return KeyCode::W;
        case SDLK_x:
            return KeyCode::X;
        case SDLK_y:
            return KeyCode::Y;
        case SDLK_z:
            return KeyCode::Z;

        //////////////////////////////////////////////////
        ///// Numeric Keys
        //////////////////////////////////////////////////
        case SDLK_0:
            return KeyCode::NUMBER_0;
        case SDLK_1:
            return KeyCode::NUMBER_1;
        case SDLK_2:
            return KeyCode::NUMBER_2;
        case SDLK_3:
            return KeyCode::NUMBER_3;
        case SDLK_4:
            return KeyCode::NUMBER_4;
        case SDLK_5:
            return KeyCode::NUMBER_5;
        case SDLK_6:
            return KeyCode::NUMBER_6;
        case SDLK_7:
            return KeyCode::NUMBER_7;
        case SDLK_8:
            return KeyCode::NUMBER_8;
        case SDLK_9:
            return KeyCode::NUMBER_9;

        //////////////////////////////////////////////////
        ///// Shift + Numeric Keys
        //////////////////////////////////////////////////
        case SDLK_EXCLAIM:
            return KeyCode::EXCLAMATION;
        case SDLK_AT:
            return KeyCode::AT;
        case SDLK_HASH:
            return KeyCode::HASH;
        case SDLK_DOLLAR:
            return KeyCode::DOLLAR;
        case SDLK_PERCENT:
            return KeyCode::PERCENT;
        case SDLK_CARET:
            return KeyCode::CARET;
        case SDLK_AMPERSAND:
            return KeyCode::AMPERSAND;
        case SDLK_ASTERISK:
            return KeyCode::ASTERISK;
        case SDLK_LEFTPAREN:
            return KeyCode::LEFT_PARAN;
        case SDLK_RIGHTPAREN:
            return KeyCode::RIGHT_PARAN;
        case SDLK_UNDERSCORE:
            return KeyCode::UNDERSCORE;
        case SDLK_PLUS:
            return KeyCode::PLUS;
        case SDLK_COLON:
            return KeyCode::COLON;
        case SDLK_QUOTEDBL:
            return KeyCode::DOUBLE_QUOTE;
        case SDLK_LESS:
            return KeyCode::LESS;
        case SDLK_GREATER:
            return KeyCode::GREATER;
        case SDLK_QUESTION:
            return KeyCode::QUESTION;

        //////////////////////////////////////////////////
        ///// Special Keys
        //////////////////////////////////////////////////
        case SDLK_RETURN:
            return KeyCode::RETURN;
        case SDLK_ESCAPE:
            return KeyCode::ESCAPE;
        case SDLK_BACKSPACE:
            return KeyCode::BACKSPACE;
        case SDLK_TAB:
            return KeyCode::TAB;
        case SDLK_SPACE:
            return KeyCode::SPACE;
        case SDLK_MINUS:
            return KeyCode::MINUS;
        case SDLK_EQUALS:
            return KeyCode::EQUALS;
        case SDLK_LEFTBRACKET:
            return KeyCode::LEFT_BRACKET;
        case SDLK_RIGHTBRACKET:
            return KeyCode::RIGHT_BRACKET;
        case SDLK_BACKSLASH:
            return KeyCode::BACKSLASH;
        case SDLK_SEMICOLON:
            return KeyCode::SEMI_COLON;
        case SDLK_QUOTE:
            return KeyCode::SINGLE_QUOTE;
        case SDLK_BACKQUOTE:
            return KeyCode::BACK_QUOTE;
        case SDLK_COMMA:
            return KeyCode::COMMA;
        case SDLK_PERIOD:
            return KeyCode::PERIOD;
        case SDLK_SLASH:
            return KeyCode::SLASH;
        case SDLK_CAPSLOCK:
            return KeyCode::CAPS_LOCK;
        case SDLK_F1:
            return KeyCode::F1;
        case SDLK_F2:
            return KeyCode::F2;
        case SDLK_F3:
            return KeyCode::F3;
        case SDLK_F4:
            return KeyCode::F4;
        case SDLK_F5:
            return KeyCode::F5;
        case SDLK_F6:
            return KeyCode::F6;
        case SDLK_F7:
            return KeyCode::F7;
        case SDLK_F8:
            return KeyCode::F8;
        case SDLK_F9:
            return KeyCode::F9;
        case SDLK_F10:
            return KeyCode::F10;
        case SDLK_F11:
            return KeyCode::F11;
        case SDLK_F12:
            return KeyCode::F12;
        case SDLK_PRINTSCREEN:
            return KeyCode::PRINT_SCREEN;
        case SDLK_SCROLLLOCK:
            return KeyCode::SCROLL_LOCK;
        case SDLK_PAUSE:
            return KeyCode::PAUSE;
        case SDLK_INSERT:
            return KeyCode::INSERT;
        case SDLK_HOME:
            return KeyCode::HOME;
        case SDLK_PAGEUP:
            return KeyCode::PAGE_UP;
        case SDLK_DELETE:
            return KeyCode::DELETE;
        case SDLK_END:
            return KeyCode::END;
        case SDLK_PAGEDOWN:
            return KeyCode::PAGE_DOWN;
        case SDLK_RIGHT:
            return KeyCode::RIGHT;
        case SDLK_LEFT:
            return KeyCode::LEFT;
        case SDLK_DOWN:
            return KeyCode::DOWN;
        case SDLK_UP:
            return KeyCode::UP;
        case SDLK_LCTRL:
            return KeyCode::LEFT_CONTROL;
        case SDLK_LSHIFT:
            return KeyCode::LEFT_SHIFT;
        case SDLK_LALT:
            return KeyCode::LEFT_ALT;
        case SDLK_RCTRL:
            return KeyCode::RIGHT_CONTROL;
        case SDLK_RSHIFT:
            return KeyCode::RIGHT_SHIFT;
        case SDLK_RALT:
            return KeyCode::RIGHT_ALT;

        //////////////////////////////////////////////////
        ///// Num pad Keys
        //////////////////////////////////////////////////
        case SDLK_KP_DIVIDE:
            return KeyCode::NUM_PAD_DIVIDE;
        case SDLK_KP_MULTIPLY:
            return KeyCode::NUM_PAD_MULTIPLY;
        case SDLK_KP_MINUS:
            return KeyCode::NUM_PAD_MINUS;
        case SDLK_KP_PLUS:
            return KeyCode::NUM_PAD_PLUS;
        case SDLK_KP_ENTER:
            return KeyCode::NUM_PAD_ENTER;
        case SDLK_KP_1:
            return KeyCode::NUM_PAD_1;
        case SDLK_KP_2:
            return KeyCode::NUM_PAD_2;
        case SDLK_KP_3:
            return KeyCode::NUM_PAD_3;
        case SDLK_KP_4:
            return KeyCode::NUM_PAD_4;
        case SDLK_KP_5:
            return KeyCode::NUM_PAD_5;
        case SDLK_KP_6:
            return KeyCode::NUM_PAD_6;
        case SDLK_KP_7:
            return KeyCode::NUM_PAD_7;
        case SDLK_KP_8:
            return KeyCode::NUM_PAD_8;
        case SDLK_KP_9:
            return KeyCode::NUM_PAD_9;
        case SDLK_KP_0:
            return KeyCode::NUM_PAD_0;
        case SDLK_KP_PERIOD:
            return KeyCode::NUM_PAD_PERIOD;
        default:
            return KeyCode::UNKNOWN;
    }
}

void EventBus::Dispatch(const SDL_Event& sdl_event)
{
	switch (sdl_event.type)
	{
		case SDL_WINDOWEVENT:
			DispatchWindowEvent(&sdl_event.window);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			DispatchKeyEvent(&sdl_event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			DispatchMouseButtonEvent(&sdl_event.button);
			break;
		case SDL_MOUSEMOTION:
			DispatchMouseMoveEvent(&sdl_event.motion);
			break;
		case SDL_MOUSEWHEEL:
			DispatchMouseWheelEvent(&sdl_event.wheel);
			break;
		default:
			break;
	}
}

void EventBus::DispatchWindowEvent(const SDL_WindowEvent* window_event)
{
    WindowEvent zero_window_event{};
    switch (window_event->type)
    {
        case SDL_WINDOWEVENT_SHOWN:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_SHOWN;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_HIDDEN;
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_EXPOSED;
            break;
        case SDL_WINDOWEVENT_MOVED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_MOVED;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_RESIZED;
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_SIZE_CHANGED;
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_MINIMIZED;
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_MAXIMIZED;
            break;
        case SDL_WINDOWEVENT_RESTORED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_RESTORED;
            break;
        case SDL_WINDOWEVENT_ENTER:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_MOUSE_ENTER;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_MOUSE_LEAVE;
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_KEYBOARD_FOCUS_GAINED;
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_KEYBOARD_FOCUS_LOST;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            zero_window_event.type_ = WindowEvent::Type::WINDOW_CLOSE;
            break;
        default:
            // Ignore unknown window events
            return;
    }

    zero_window_event.x_ = window_event->data1;
    zero_window_event.y_ = window_event->data2;
    for (const auto& event_handler : window_event_listeners_)
    {
        event_handler->OnWindowEvent(zero_window_event);
    }
}

void EventBus::DispatchKeyEvent(const SDL_KeyboardEvent* keyboard_event)
{
    KeyCode key_code = SDLKeyCodeToZeroKeyCode(keyboard_event->keysym.sym);
    if (keyboard_event->state == SDL_PRESSED)
    {
        for (const auto& event_handler : key_pressed_listeners_)
        {
            event_handler->OnKeyPressed(key_code);
        }
    }
    else
    {
        for (const auto& event_handler : key_released_listeners_)
        {
            event_handler->OnKeyReleased(key_code);
        }
    }
}

void EventBus::DispatchMouseButtonEvent(const SDL_MouseButtonEvent* mouse_button_event)
{
    MouseButtonEvent zero_mouse_button_event{};
    switch (mouse_button_event->button)
    {
        case SDL_BUTTON_LEFT:
            zero_mouse_button_event.mouse_button = MouseButton::LEFT;
            break;
        case SDL_BUTTON_MIDDLE:
            zero_mouse_button_event.mouse_button = MouseButton::MIDDLE;
            break;
        case SDL_BUTTON_RIGHT:
            zero_mouse_button_event.mouse_button = MouseButton::RIGHT;
            break;
        case SDL_BUTTON_X1:
            zero_mouse_button_event.mouse_button = MouseButton::EXTRA_BUTTON_1;
            break;
        case SDL_BUTTON_X2:
            zero_mouse_button_event.mouse_button = MouseButton::EXTRA_BUTTON_2;
            break;
        default:
            // Ignore unknown button events
            return;
    }
    zero_mouse_button_event.x_ = mouse_button_event->x;
    zero_mouse_button_event.y_ = mouse_button_event->y;
    zero_mouse_button_event.click_count_ = mouse_button_event->clicks;
    if (mouse_button_event->state == SDL_PRESSED)
    {
        for (const auto& event_handler : mouse_button_pressed_listeners_)
        {
            event_handler->OnMouseButtonPressed(zero_mouse_button_event);
        }
    }
    else
    {
        for (const auto& event_handler : mouse_button_released_listeners_)
        {
            event_handler->OnMouseButtonReleased(zero_mouse_button_event);
        }
    }
}

void EventBus::DispatchMouseMoveEvent(const SDL_MouseMotionEvent* mouse_motion_event)
{
    MouseMoveEvent zero_mouse_move_event{};
    zero_mouse_move_event.x_ = mouse_motion_event->x;
    zero_mouse_move_event.y_ = mouse_motion_event->y;
    zero_mouse_move_event.relative_x_motion_ = mouse_motion_event->xrel;
    zero_mouse_move_event.relative_y_motion_ = mouse_motion_event->yrel;
    for (const auto& event_handler : mouse_move_listeners_)
    {
        event_handler->OnMouseMoved(zero_mouse_move_event);
    }
}

void EventBus::DispatchMouseWheelEvent(const SDL_MouseWheelEvent* mouse_wheel_event)
{
    MouseWheelEvent zero_mouse_wheel_event{};
    zero_mouse_wheel_event.x_ = mouse_wheel_event->x;
    zero_mouse_wheel_event.y_ = mouse_wheel_event->y;
    for (const auto& event_handler : mouse_wheel_listeners_)
    {
        event_handler->OnMouseWheelScroll(zero_mouse_wheel_event);
    }
}

void EventBus::RegisterWindowEventListener(EventHandler* event_handler)
{
	window_event_listeners_.push_back(event_handler);
}

void EventBus::RegisterKeyPressedListener(EventHandler* event_handler)
{
	key_pressed_listeners_.push_back(event_handler);
}

void EventBus::RegisterKeyReleasedListener(EventHandler* event_handler)
{
	key_released_listeners_.push_back(event_handler);
}

void EventBus::RegisterMouseButtonPressedListener(EventHandler* event_handler)
{
	mouse_button_pressed_listeners_.push_back(event_handler);
}

void EventBus::RegisterMouseButtonReleasedListener(EventHandler* event_handler)
{
	mouse_button_released_listeners_.push_back(event_handler);
}

void EventBus::RegisterMouseMoveListener(EventHandler* event_handler)
{
	mouse_move_listeners_.push_back(event_handler);
}

void EventBus::RegisterMouseWheelListener(EventHandler* event_handler)
{
	mouse_wheel_listeners_.push_back(event_handler);
}

} // namespace zero