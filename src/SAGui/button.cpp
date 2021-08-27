#include <iostream>
#include <algorithm>
#include <map>
#include "button.h"

namespace SA
{
    struct Button::ButtonPrivate
    {
        std::string text = "Button 1";
        bool pressed = false;
        std::map<int, std::function<void (bool)> > pressHanders;
        std::map<int, std::function<void (bool)> > hoverHanders;
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

    void Button::setText(const std::string &text)
    {
        d->text = text;
        update();
    }

    std::string Button::text()
    {
        return d->text;
    }

    int Button::addPressHandler(const std::function<void(bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->pressHanders)
            if (it.first != ++id) break;

        d->pressHanders.insert({id, func});
        return id;
    }

    void Button::removePressHandler(int id)
    {
        auto it = d->pressHanders.find(id);
        if (it != d->pressHanders.end())
            d->pressHanders.erase(it);
    }

    int Button::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->hoverHanders)
            if (it.first != ++id) break;

        d->hoverHanders.insert({id, func});
        return id;
    }

    void Button::removeHoverHandler(int id)
    {
        auto it = d->hoverHanders.find(id);
        if (it != d->hoverHanders.end())
            d->hoverHanders.erase(it);
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

        for (const auto &it: d->hoverHanders)
            it.second(state);
    }

    void Button::mousePressEvent(bool state, unsigned int button)
    {
        if (button != ButtonLeft) return;

        d->pressed = state;
        update();

        for (const auto &it: d->pressHanders)
            it.second(state);
    }
}
