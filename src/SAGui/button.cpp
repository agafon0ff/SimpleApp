#include <iostream>
#include <algorithm>
#include <map>
#include "button.h"

namespace SA
{
    struct Button::ButtonPrivate
    {
        std::string text = "Button";
        bool enable = true;
        bool pressed = false;
        bool checked = false;
        bool checkable = false;

        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Brush textColors[AllStates];
        Brush backgrounds[AllStates];

        std::map<int, std::function<void (bool)> > hoverHanders;
        std::map<int, std::function<void (bool)> > pressHanders;
        std::map<int, std::function<void (bool)> > checkHanders;
    };

    Button::Button(Widget *parent) : Button("Button", parent)
    {
    }

    Button::Button(const std::string &text, Widget *parent) : Widget(parent),
        d(new ButtonPrivate)
    {
        d->text = text;
        resize(150, 40);
        calcTextColors({20, 20, 20});
        calcBorders({90, 90, 90, 1});
        calcBackgrounds({250, 250, 250});
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
        return std::move(d->text);
    }

    void Button::setEnabled(bool state)
    {
        d->enable = state;
        d->styleState = state ? EnableState : DisableState;
        update();
    }

    bool Button::isEnabled()
    {
        return d->enable;
    }

    void Button::setCheckable(bool state)
    {
        d->checkable = state;
    }

    bool Button::isCheckable()
    {
        return d->checkable;
    }

    void Button::setChecked(bool state)
    {
        d->checked = state;
        update();
    }

    bool Button::isChecked()
    {
        return d->checked;
    }

    bool Button::isPressed()
    {
        return d->pressed;
    }

    void Button::setTextColor(unsigned char red, unsigned char green,
                              unsigned char blue, StyleState state)
    {
        if (state == AllStates) calcTextColors({red, green, blue});
        else d->textColors[state] = {red, green, blue};
    }

    void Button::setBorder(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned int width, StyleState state)
    {
        if (state == AllStates) calcBorders({red, green, blue, width});
        else d->borderPens[state] = {red, green, blue, width};
    }

    void Button::setBackground(unsigned char red, unsigned char green,
                               unsigned char blue, StyleState state)
    {
        if (state == AllStates) calcBackgrounds({red, green, blue});
        else d->backgrounds[state] = {red, green, blue};
    }

    int Button::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->hoverHanders) if (it.first != ++id) break;
        d->hoverHanders.insert({id, func});
        return id;
    }

    void Button::removeHoverHandler(int id)
    {
        auto it = d->hoverHanders.find(id);
        if (it != d->hoverHanders.end())
            d->hoverHanders.erase(it);
    }

    int Button::addPressHandler(const std::function<void(bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->pressHanders) if (it.first != ++id) break;
        d->pressHanders.insert({id, func});
        return id;
    }

    void Button::removePressHandler(int id)
    {
        auto it = d->pressHanders.find(id);
        if (it != d->pressHanders.end())
            d->pressHanders.erase(it);
    }

    int Button::addCheckHandler(const std::function<void (bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->checkHanders) if (it.first != ++id) break;
        d->checkHanders.insert({id, func});
        return id;
    }

    void Button::removeCheckHandler(int id)
    {
        auto it = d->checkHanders.find(id);
        if (it != d->checkHanders.end())
            d->checkHanders.erase(it);
    }

    void Button::paintEvent()
    {
        Brush brush = d->backgrounds[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);

        Pen pen = d->borderPens[d->styleState];
        setPen(pen.red, pen.green, pen.blue, pen.width);
        drawRect(0, 0, width() - 1, height() - 1);

        brush = d->textColors[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);
        drawText(width() / 2 - textWidth(d->text) / 2,
                 height() / 2 - textHeight() / 2,
                 d->text);
    }

    void Button::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = d->checked ? CheckedState :
                                     state ? HoveredState : EnableState;
        update();
        for (const auto &it: d->hoverHanders) it.second(state);
    }

    void Button::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;

        d->pressed = event.pressed;

        if (d->checkable)
        {
            if (event.pressed)
            {
                d->checked = !d->checked;
                d->styleState = d->checked ? CheckedState : HoveredState;
            }
        }
        else
        {
            d->checked = event.pressed;
            d->styleState = d->checked ? PressedState : HoveredState;
        }

        update();

        for (const auto &it: d->pressHanders) it.second(event.pressed);

        if (d->checkable)
        { if(event.pressed) for (const auto &it: d->checkHanders) it.second(d->checked); }
        else for (const auto &it: d->checkHanders) it.second(d->checked);
    }

    void Button::calcTextColors(const Brush &brush)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->textColors[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((brush.red - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.green - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.blue - 20 * i), 0, 255))
                };
        }
    }

    void Button::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->borderPens[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((pen.red - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((pen.green - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((pen.blue - 20 * i), 0, 255)),
                    pen.width
                };
        }
    }

    void Button::calcBackgrounds(const Brush &brush)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->backgrounds[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((brush.red - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.green - 20 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.blue - 20 * i), 0, 255))
                };
        }
    }
}
