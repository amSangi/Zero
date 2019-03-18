#pragma once

namespace zero {
namespace input {

    /**
     * @brief Virtual Keys
     */
    enum Key {
        /* ********** Alphabet ********** */
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,

        /* ********** Special Keys ********** */
        KEY_LEFT_PARAN,                           // (
        KEY_RIGHT_PARAN,                          // )
        KEY_LEFT_BRACE,                           // {
        KEY_RIGHT_BRACE,                          // }
        KEY_LEFT_SQUARE_BRACE,                    // [
        KEY_RIGHT_SQUARE_BRACE,                   // ]
        KEY_LEFT_ANGLE_BRACE,                     // <
        KEY_RIGHT_ANGLE_BRACE,                    // >
        KEY_DOT,                                  // .
        KEY_COMMA,                                // ,
        KEY_EXCLAMATION,                          // !
        KEY_AT,                                   // @
        KEY_NUMBER_SIGN,                          // #
        KEY_DOLLAR_SIGN,                          // $
        KEY_PERCENT,                              // %
        KEY_CARET,                                // ^
        KEY_AMPERSAND,                            // &
        KEY_ASTERISK,                             // *
        KEY_DOUBLE_QUOTE,                         // "
        KEY_SINGLE_QUOTE,                         // '
        KEY_MINUS,                                // -
        KEY_UNDERSCORE,                           // _
        KEY_PLUS,                                 // +
        KEY_EQUAL,                                // =
        KEY_QUESTION_MARK,                        // ?
        KEY_COLON,                                // :
        KEY_SEMI_COLON,                           // ;
        KEY_BACK_QUOTE,                           // `
        KEY_TILDE,                                // ~
        KEY_FORWARD_SLASH,                        // /
        KEY_BACK_SLASH,                           // \

        /* ********** Numbers ********** */
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,

        /* ********** Function Keys ********** */
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,

        /* ********** Utility Keys ********** */
        KEY_SPACE,
        KEY_ESCAPE,
        KEY_TAB,
        KEY_CAPS_LOCK,
        KEY_NUM_LOCK,
        KEY_LEFT_SHIFT,
        KEY_RIGHT_SHIFT,
        KEY_LEFT_CONTROL,
        KEY_RIGHT_CONTROL,
        KEY_LEFT_ALT,
        KEY_RIGHT_ALT,
        KEY_RETURN,
        KEY_ENTER,
        KEY_INSERT,
        KEY_PAUSE,
        KEY_DELETE,
        KEY_BACKSPACE,
        KEY_BACKTAB,
        KEY_HOME,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,
        KEY_PRINT_SCREEN,
        KEY_SCROLL_LOCK,
        KEY_COMMAND,
        KEY_FUNCTION,

        /* ********** Arrow Keys ********** */
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,

        KEY_COUNT,
    }; // enum Key

} // namespace input
} // namespace zero