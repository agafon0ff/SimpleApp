#pragma once

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

} // namespace SA
