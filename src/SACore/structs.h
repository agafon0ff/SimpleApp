#pragma once
#include "global.h"
#include <iostream>

namespace SA
{
    struct Color
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;

        Color(uint8_t red_, uint8_t green_, uint8_t blue_):
            red(red_), green(green_), blue(blue_){}

        Color(){}
    };

    struct Pen
    {
        Color color;
        uint8_t width = 1;

        Pen(uint8_t red_, uint8_t green_,
            uint8_t blue_, uint32_t width_):
            color(red_, green_, blue_), width(width_){}

        Pen(){}
    };

    struct Point
    {
        int32_t x = 0;
        int32_t y = 0;

        Point(int x_, int y_): x(x_), y(y_) {}
        Point(){}

        friend std::ostream &operator<< (std::ostream &output, const Point &p)
        {
            output << "x: " << p.x << ", y: " << p.y;
            return output;
        }
    };

    struct Size
    {
        uint32_t width = 0;
        uint32_t height = 0;

        Size(uint32_t width_, uint32_t height_):
            width(width_), height(height_) {}

        Size(){}

        friend std::ostream &operator<< (std::ostream &output, const Size &s)
        {
            output << "w: " << s.width << ", h: " << s.height;
            return output;
        }
    };

    struct Rect
    {
        int32_t x = 0;
        int32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;

        Rect(int32_t x_, int32_t y_, uint32_t width_, uint32_t height_):
            x(x_), y(y_), width(width_), height(height_) {}

        Rect(){}
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

    struct TextSelection
    {
        uint32_t posStart = 0;
        uint32_t posEnd = 0;

        uint32_t rowStart = 0;
        uint32_t rowEnd = 0;

        uint32_t columnStart = 0;
        uint32_t columnEnd = 0;

        bool selected = false;
    };

    struct TextAction
    {
        enum ChangeType
        {
            Null,
            InsertChar,
            InsertText,
            RemoveChar,
            RemoveText
        } type = Null;

        uint64_t pos = 0;
        char symbol = ' ';
        std::string string;

        TextAction(ChangeType type_, uint64_t pos_, char symbol_) :
            type(type_), pos(pos_), symbol(symbol_) {}

        TextAction(ChangeType type_, uint64_t pos_, const std::string &string_) :
            type(type_), pos(pos_), string(string_){}

        TextAction(const TextAction &a) :
            type(a.type), pos(a.pos), symbol(a.symbol), string(a.string) {}

        TextAction(TextAction &&a) noexcept :
        type(a.type), pos(a.pos), symbol(a.symbol), string(std::move(a.string)) {}

        TextAction& operator=(const TextAction &a)
        {if (&a == this) return *this;  type = a.type; pos = a.pos;
         symbol = a.symbol; string = a.string; return *this;}

        TextAction& operator=(TextAction &&a) noexcept
        {if (&a == this) return *this;  type = a.type; pos = a.pos;
         symbol = a.symbol; string = std::move(a.string); return *this;}
    };

} // namespace SA
