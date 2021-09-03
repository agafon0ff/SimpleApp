#pragma once
#include "SACore/global.h"

namespace SA
{
    struct Brush
    {
        unsigned char red = 0;
        unsigned char green = 0;
        unsigned char blue = 0;

        Brush(unsigned char red_, unsigned char green_, unsigned char blue_):
            red(red_), green(green_), blue(blue_){}

        Brush(){}
    };

    struct Pen
    {
        unsigned char red = 0;
        unsigned char green = 0;
        unsigned char blue = 0;
        unsigned int width = 1;

        Pen(unsigned char red_, unsigned char green_,
            unsigned char blue_, unsigned int width_):
            red(red_), green(green_), blue(blue_), width(width_){}

        Pen(){}
    };

    struct KeyModifiers
    {
        bool shift = false;
        bool alt = false;
        bool ctrl = false;
        bool super = false;
        bool capsLock = false;
        bool numLock = false;
    };

    struct KeyEvent
    {
        Keys keycode;
        KeyModifiers modifiers;
        bool pressed = false;

        KeyEvent(Keys keycode_, KeyModifiers modifiers_, bool pressed_) :
            keycode(keycode_), modifiers(modifiers_), pressed(pressed_){}
    };

    struct MouseEvent
    {
        MouseButton button;
        bool pressed = false;

        MouseEvent(MouseButton button_, bool pressed_) :
            button(button_), pressed(pressed_){}
    };

} // namespace SA
