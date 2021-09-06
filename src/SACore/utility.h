#pragma once

#include "SACore/global.h"
#include "SACore/structs.h"

namespace SA
{
    inline bool operator==(const Point &p1, const Point &p2) { return (p1.x == p2.x && p1.y == p2.y); }
    inline bool operator!=(const Point &p1, const Point &p2){ return (p1.x != p2.x || p1.y != p2.y); }
    inline const Point operator-(const Point &p1, const Point &p2){ return Point(p1.x - p2.x, p1.y - p2.y); }
    inline const Point operator+(const Point &p1, const Point &p2){ return Point(p1.x + p2.x, p1.y + p2.y); }
    inline const Point operator*(const Point &p, double c){ return Point(p.x * c, p.y * c); }

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

} // namespace SA
