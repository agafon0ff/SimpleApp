#include <iostream>
#include <algorithm>
#include <map>

#include "button.h"
#include "utility.h"

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
        Color textColors[AllStates];
        Color backgrounds[AllStates];

        std::map<int, std::function<void (bool)> > hoverHandlers;
        std::map<int, std::function<void (bool)> > pressHandlers;
        std::map<int, std::function<void (bool)> > checkHandlers;
    };

    Button::Button(Widget *parent) : Button("Button", parent)
    {
    }

    Button::Button(const std::string &text, Widget *parent) : Widget(parent),
        d(new ButtonPrivate)
    {
        d->text = text;
        resize(150, 40);
        calcTextColors({250, 250, 250});
        calcBorders({90, 90, 90, 1});
        calcBackgrounds({70, 70, 70});
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

    void Button::setTextColor(const Color &color, StyleState state)
    {
        if (state == AllStates) calcTextColors(color);
        else d->textColors[state] = color;
    }

    void Button::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
    }

    void Button::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates) calcBackgrounds(color);
        else d->backgrounds[state] = color;
    }

    int Button::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = static_cast<int>(d->hoverHandlers.size());
        for (auto const& it : d->hoverHandlers) if (it.first != ++id) break;
        d->hoverHandlers.insert({id, func});
        return id;
    }

    void Button::removeHoverHandler(int id)
    {
        auto it = d->hoverHandlers.find(id);
        if (it != d->hoverHandlers.end())
            d->hoverHandlers.erase(it);
    }

    int Button::addPressHandler(const std::function<void(bool)> &func)
    {
        int id = static_cast<int>(d->pressHandlers.size());
        for (auto const& it : d->pressHandlers) if (it.first != ++id) break;
        d->pressHandlers.insert({id, func});
        return id;
    }

    void Button::removePressHandler(int id)
    {
        auto it = d->pressHandlers.find(id);
        if (it != d->pressHandlers.end())
            d->pressHandlers.erase(it);
    }

    int Button::addCheckHandler(const std::function<void (bool)> &func)
    {
        int id = static_cast<int>(d->checkHandlers.size());
        for (auto const& it : d->checkHandlers) if (it.first != ++id) break;
        d->checkHandlers.insert({id, func});
        return id;
    }

    void Button::removeCheckHandler(int id)
    {
        auto it = d->checkHandlers.find(id);
        if (it != d->checkHandlers.end())
            d->checkHandlers.erase(it);
    }

    void Button::paintEvent()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);

        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 1);
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
        for (const auto &it: d->hoverHandlers) it.second(state);
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

        for (const auto &it: d->pressHandlers) it.second(event.pressed);

        if (d->checkable)
        { if(event.pressed) for (const auto &it: d->checkHandlers) it.second(d->checked); }
        else for (const auto &it: d->checkHandlers) it.second(d->checked);
    }

    void Button::calcTextColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->textColors[static_cast<StyleState>(i)], 2 * i);
    }

    void Button::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(pen.color, d->borderPens[static_cast<StyleState>(i)].color, 6 * i);
    }

    void Button::calcBackgrounds(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->backgrounds[static_cast<StyleState>(i)], 6 * i);
    }
}
