#pragma once

namespace zero
{

    /**
     * @brief Virtual keys
     */
    enum class KeyCode
    {
        UNKNOWN = 0,              ///< An unknown key

        //////////////////////////////////////////////////
        ///// Alphabet Keys
        //////////////////////////////////////////////////
        A,                        ///< 'a' key
        B,                        ///< 'b' key
        C,                        ///< 'c' key
        D,                        ///< 'd' key
        E,                        ///< 'e' key
        F,                        ///< 'f' key
        G,                        ///< 'g' key
        H,                        ///< 'h' key
        I,                        ///< 'i' key
        J,                        ///< 'j' key
        K,                        ///< 'k' key
        L,                        ///< 'l' key
        M,                        ///< 'm' key
        N,                        ///< 'n' key
        O,                        ///< 'o' key
        P,                        ///< 'p' key
        Q,                        ///< 'q' key
        R,                        ///< 'r' key
        S,                        ///< 's' key
        T,                        ///< 't' key
        U,                        ///< 'u' key
        V,                        ///< 'v' key
        W,                        ///< 'w' key
        X,                        ///< 'x' key
        Y,                        ///< 'y' key
        Z,                        ///< 'z' key

        //////////////////////////////////////////////////
        ///// Numeric Keys
        //////////////////////////////////////////////////
        NUMBER_1,                  ///< '1' key
        NUMBER_2,                  ///< '2' key
        NUMBER_3,                  ///< '3' key
        NUMBER_4,                  ///< '4' key
        NUMBER_5,                  ///< '5' key
        NUMBER_6,                  ///< '6' key
        NUMBER_7,                  ///< '7' key
        NUMBER_8,                  ///< '8' key
        NUMBER_9,                  ///< '9' key
        NUMBER_0,                  ///< '0' key

        //////////////////////////////////////////////////
        ///// Shift + Numeric Keys
        //////////////////////////////////////////////////
        EXCLAMATION,              ///< '!' key
        AT,                       ///< '@' key
        HASH,                     ///< '#' key
        DOLLAR,                   ///< '$' key
        PERCENT,                  ///< '%' key
        CARET,                    ///< '^' key
        AMPERSAND,                ///< '&' key
        ASTERISK,                 ///< '*' key
        LEFT_PARAN,               ///< '(' key
        RIGHT_PARAN,              ///< ')' key
        UNDERSCORE,               ///< '_' key
        PLUS,                     ///< '+' key
        COLON,                    ///< ':' key
        DOUBLE_QUOTE,             ///< '"' key
        LESS,                     ///< '<' key
        GREATER,                  ///< '>' key
        QUESTION,                 ///< '?' key

        //////////////////////////////////////////////////
        ///// Special Keys
        //////////////////////////////////////////////////
        RETURN,                   ///< return key
        ESCAPE,                   ///< escape key
        BACKSPACE,                ///< backspace key
        TAB,                      ///< tab key
        SPACE,                    ///< space key

        MINUS,                    ///< '-' key
        EQUALS,                   ///< '=' key
        LEFT_BRACKET,             ///< '[' key
        RIGHT_BRACKET,            ///< ']' key
        BACKSLASH,                ///< '\' key

        SEMI_COLON,               ///< ';' key
        SINGLE_QUOTE,             ///< ''' key
        BACK_QUOTE,               ///< '`' key
        COMMA,                    ///< ',' key
        PERIOD,                   ///< '.' key
        SLASH,                    ///< '/' key
        CAPS_LOCK,                ///< caps lock key
        F1,                       ///< 'F1' key
        F2,                       ///< 'F2' key
        F3,                       ///< 'F3' key
        F4,                       ///< 'F4' key
        F5,                       ///< 'F5' key
        F6,                       ///< 'F6' key
        F7,                       ///< 'F7' key
        F8,                       ///< 'F8' key
        F9,                       ///< 'F9' key
        F10,                      ///< 'F10' key
        F11,                      ///< 'F11' key
        F12,                      ///< 'F12' key

        PRINT_SCREEN,             ///< print screen key
        SCROLL_LOCK,              ///< scroll lock key
        PAUSE,                    ///< pause key
        INSERT,                   ///< insert key
        HOME,                     ///< home key
        PAGE_UP,                  ///< page up key
        DELETE,                   ///< delete key
        END,                      ///< end key
        PAGE_DOWN,                ///< page down key
        RIGHT,                    ///< right arrow key
        LEFT,                     ///< left arrow key
        DOWN,                     ///< down arrow key
        UP,                       ///< up arrow key

        LEFT_CONTROL,             ///< left control key
        LEFT_SHIFT,               ///< left shift key
        LEFT_ALT,                 ///< left alt key
        RIGHT_CONTROL,            ///< right control key
        RIGHT_SHIFT,              ///< right shift key
        RIGHT_ALT,                ///< right alt key

        //////////////////////////////////////////////////
        ///// Num pad Keys
        //////////////////////////////////////////////////
        NUM_PAD_DIVIDE,           ///< '/' num pad key
        NUM_PAD_MULTIPLY,         ///< '*' num pad key
        NUM_PAD_MINUS,            ///< '-' num pad key
        NUM_PAD_PLUS,             ///< '+' num pad key
        NUM_PAD_ENTER,            ///< num pad enter key
        NUM_PAD_1,                ///< '1' num pad key
        NUM_PAD_2,                ///< '2' num pad key
        NUM_PAD_3,                ///< '3' num pad key
        NUM_PAD_4,                ///< '4' num pad key
        NUM_PAD_5,                ///< '5' num pad key
        NUM_PAD_6,                ///< '6' num pad key
        NUM_PAD_7,                ///< '7' num pad key
        NUM_PAD_8,                ///< '8' num pad key
        NUM_PAD_9,                ///< '9' num pad key
        NUM_PAD_0,                ///< '0' num pad key
        NUM_PAD_PERIOD,           ///< '.' num pad key

    }; // enum class Key

} // namespace zero