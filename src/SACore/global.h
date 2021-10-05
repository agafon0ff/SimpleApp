#pragma once

#include <cstdint>
#include <map>

namespace SA
{
    enum EventTypes
    {
        KeyboardEvent,
        MouseMoveEvent,
        MouseHoverEvent,
        MouseButtonEvent,
        MouseWheelEvent,
        FocusInEvent,
        FocusOutEvent,
        PaintEvent,
        MoveEvent,
        ResizeEvent
    };

    enum MouseButton
    {
        ButtonLeft,
        ButtonRight,
        ButtonMiddle,
        ButtonX1,
        ButtonX2
    };

    enum CursorShapes
    {
        Arrow,
        Text
    };

    enum Direction
    {
        DirLeft,
        DirRight,
        DirUp,
        DirDown
    };

    enum Side
    {
        SideLeft,
        SideTop,
        SideRight,
        SideBottom
    };

    enum Orientation
    {
        Vertical,
        Horizontal
    };

    enum StyleState
    {
        DisableState,
        EnableState,
        HoveredState,
        AllStates
    };

    enum Keys
    {
        Key_Escape,     Key_Tab,         Key_Backspace,  Key_Return,    Key_Enter,
        Key_Insert,     Key_Delete,      Key_Pause,      Key_Print,     Key_SysReq,
        Key_Clear,      Key_Home,        Key_End,        Key_Left,      Key_Up,
        Key_Right,      Key_Down,        Key_PageUp,     Key_PageDown,  Key_Shift,
        Key_LShift,     Key_RShift,      Key_Control,    Key_ControlL,  Key_ControlR,
        Key_Alt,        Key_AltR,        Key_AltL,       Key_CapsLock,  Key_NumLock,
        Key_ScrollLock,
        Key_F1,         Key_F2,          Key_F3,         Key_F4,
        Key_F5,         Key_F6,          Key_F7,         Key_F8,        Key_F9,
        Key_F10,        Key_F11,         Key_F12,        Key_F13,       Key_F14,
        Key_F15,        Key_F16,         Key_F17,        Key_F18,       Key_F19,
        Key_F20,        Key_F21,         Key_F22,        Key_F23,       Key_F24,

        Key_Menu,       Key_LMenu,       Key_RMenu,      Key_Help,      Key_Space,
        Key_Quote,      Key_Dollar,      Key_Percent,    Key_Ampersand, Key_Apostrophe,
        Key_ParenLeft,  Key_ParenRight,  Key_Asterisk,   Key_Plus,      Key_Comma,
        Key_Minus,      Key_Period,      Key_Slash,

        Key_0,        Key_1,        Key_2,        Key_3,
        Key_4,        Key_5,        Key_6,        Key_7,
        Key_8,        Key_9,

        Key_Colon,     Key_Semicolon,    Key_Less,     Key_Equal,     Key_Greater,
        Key_Question,   Key_At,

        Key_A,  Key_B,  Key_C,  Key_D,  Key_E,  Key_F,  Key_G,  Key_H,
        Key_I,  Key_J,  Key_K,  Key_L,  Key_M,  Key_N,  Key_O,  Key_P,
        Key_Q,  Key_R,  Key_S,  Key_T,  Key_U,  Key_V,  Key_W,  Key_X,
        Key_Y,  Key_Z,

        Key_BracketLeft,    Key_Backslash,     Key_BracketRight,   Key_AsciiCircum,
        Key_Underscore,     Key_QuoteLeft,     Key_QuoteRight,     Key_BraceLeft,
        Key_Bar,            Key_BraceRight,    Key_AsciiTilde,     Key_VolumeMute,
        Key_VolumeDown,     Key_VolumeUp,      Key_MediaNext,      Key_MediaPrev,
        Key_MediaStop,      Key_MediaPlay,

        Key_Unknown

    }; // enum Keys

    static std::map<Keys, char> CHARS_LOWERCASE =
    {
        { Key_A, 'a' }, { Key_B, 'b' }, { Key_C, 'c' }, { Key_D, 'd' }, { Key_E, 'e' },
        { Key_F, 'f' }, { Key_G, 'g' }, { Key_H, 'h' }, { Key_I, 'i' }, { Key_J, 'j' },
        { Key_K, 'k' }, { Key_L, 'l' }, { Key_M, 'm' }, { Key_N, 'n' }, { Key_O, 'o' },
        { Key_P, 'p' }, { Key_Q, 'q' }, { Key_R, 'r' }, { Key_S, 's' }, { Key_T, 't' },
        { Key_U, 'u' }, { Key_V, 'v' }, { Key_W, 'w' }, { Key_X, 'x' }, { Key_Y, 'y' },
        { Key_Z, 'z' }
    }; // CHARS_LOWERCASE

    static std::map<Keys, char> CHARS_UPPERCASE =
    {
        { Key_A, 'A' }, { Key_B, 'B' }, { Key_C, 'C' }, { Key_D, 'D' }, { Key_E, 'E' },
        { Key_F, 'F' }, { Key_G, 'G' }, { Key_H, 'H' }, { Key_I, 'I' }, { Key_J, 'J' },
        { Key_K, 'K' }, { Key_L, 'L' }, { Key_M, 'M' }, { Key_N, 'N' }, { Key_O, 'O' },
        { Key_P, 'P' }, { Key_Q, 'Q' }, { Key_R, 'R' }, { Key_S, 'S' }, { Key_T, 'T' },
        { Key_U, 'U' }, { Key_V, 'V' }, { Key_W, 'W' }, { Key_X, 'X' }, { Key_Y, 'Y' },
        { Key_Z, 'Z' }
    }; // CHARS_UPPERCASE

    static std::map<Keys, char> SYMBOLS_LOWERCASE =
    {
        { Key_0, '0' }, { Key_1, '1' }, { Key_2, '2' }, { Key_3, '3' }, { Key_4, '4' },
        { Key_5, '5' }, { Key_6, '6' }, { Key_7, '7' }, { Key_8, '8' }, { Key_9, '9' },

        { Key_QuoteLeft, '`' },   { Key_Minus, '-'},         { Key_Equal, '='},
        { Key_BracketLeft, '[' }, { Key_BracketRight, ']' }, { Key_Backslash, '\\'},
        { Key_Semicolon, ';' },   { Key_QuoteRight, '\'' },  { Key_Comma, ',' },
        { Key_Period, '.' },      { Key_Slash, '/' },        {Key_Space, ' '}
    }; // SYMBOLS_LOWERCASE

    static std::map<Keys, char> SYMBOLS_UPPERCASE =
    {
        { Key_0, ')' }, { Key_1, '!' }, { Key_2, '@' }, { Key_3, '#' }, { Key_4, '$' },
        { Key_5, '%' }, { Key_6, '^' }, { Key_7, '&' }, { Key_8, '*' }, { Key_9, '(' },

        { Key_QuoteLeft, '~' },   { Key_Minus, '_'},         { Key_Equal, '+'},
        { Key_BracketLeft, '{' }, { Key_BracketRight, '}' }, { Key_Backslash, '|'},
        { Key_Semicolon, ':' },   { Key_QuoteRight, '\"' },  { Key_Comma, '<' },
        { Key_Period, '>' },      { Key_Slash, '?' },        {Key_Space, ' '}
    }; // SYMBOLS_UPPERCASE

} // namespace SA
