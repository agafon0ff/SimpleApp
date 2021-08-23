#include <iostream>
#include "button.h"

namespace SA
{
    struct Button::ButtonPrivate
    {
        std::string text = "Button 1";
        bool pressed = false;
    };

    Button::Button(Widget *parent) : Widget(parent),
        d(new ButtonPrivate)
    {
        resize(150, 40);
    }

    Button::~Button()
    {
        delete d;
    }

    void Button::paintEvent()
    {
        setPen(1, 0, 0, 0);

        if (isHovered())
            setBrush(200, 200, 200);
        else setBrush(220, 220, 220);

        if (d->pressed) setBrush(180, 180, 180);

        drawRect(0, 0, width() - 1, height() - 1);
        drawText(width() / 2 - textWidth(d->text) / 2,
                 height() / 2 - textHeight() / 2,
                 d->text);
    }

    void Button::mouseHoverEvent(bool state)
    {
        update();
    }

    void Button::mousePressEvent(bool state, unsigned int button)
    {
        if (button != ButtonLeft) return;

        d->pressed = state;
        update();
    }
}
