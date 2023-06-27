#pragma once

#include <algorithm>

#include "global.h"
#include "structs.h"

namespace SA
{
    inline bool operator==(const Point &p1, const Point &p2) { return (p1.x == p2.x && p1.y == p2.y); }
    inline bool operator!=(const Point &p1, const Point &p2){ return (p1.x != p2.x || p1.y != p2.y); }
    inline const Point operator-(const Point &p1, const Point &p2){ return Point(p1.x - p2.x, p1.y - p2.y); }
    inline const Point operator+(const Point &p1, const Point &p2){ return Point(p1.x + p2.x, p1.y + p2.y); }
    inline const Point operator*(const Point &p, double c){ return Point(p.x * c, p.y * c); }

    inline const bool contains(const Rect &rect, const Point &point)
    {
        return point.x >= rect.x && point.y >= rect.y &&
               point.x <= rect.width + rect.x &&
               point.y <= rect.height + rect.y;
    }

    inline char getCharacter(const KeyEvent &event)
    {
        char result = 0;
        bool isChar = (event.keycode >= Key_A && event.keycode <= Key_Z);

        if (isChar)
        {
            if (event.modifiers.shift || event.modifiers.capsLock)
            {
                if (CHARS_UPPERCASE.find(event.keycode) != CHARS_UPPERCASE.end())
                    return CHARS_UPPERCASE[event.keycode];
            }
            else
            {
                if (CHARS_LOWERCASE.find(event.keycode) != CHARS_LOWERCASE.end())
                    return CHARS_LOWERCASE[event.keycode];
            }
        }
        else
        {
            if (event.modifiers.shift)
            {
                if (SYMBOLS_UPPERCASE.find(event.keycode) != SYMBOLS_UPPERCASE.end())
                    return SYMBOLS_UPPERCASE[event.keycode];
            }
            else
            {
                if (SYMBOLS_LOWERCASE.find(event.keycode) != SYMBOLS_LOWERCASE.end())
                    return SYMBOLS_LOWERCASE[event.keycode];
            }
        }

        return result;
    }

    inline void darker(const Color &src, Color &dst, unsigned char subtract)
    {
        dst.red   = std::clamp((src.red - subtract), 0, 255);
        dst.green = std::clamp((src.green - subtract), 0, 255);
        dst.blue  = std::clamp((src.blue - subtract), 0, 255);
    }

    inline void darker(Color &color, unsigned char subtract){ darker(color, color, subtract); }

    template <class T>
    inline void rgba2bgra(T &array)
    {
        if (array.size() % 4 != 0) return;

        for(int i = 0; i < array.size(); i += 4)
        {
           uint8_t red  = array[i+2];
           uint8_t blue = array[i];
           array[i]   = red;
           array[i+2] = blue;
        }
    }

} // namespace SA
