#include <algorithm>
#include <iostream>

#include "utility.h"
#include "scrollbar.h"

static const uint16_t MIN_HANDLE_SIZE = 30;

using std::cout;
using std::endl;

namespace SA
{

    struct ScrollBar::ScrollBarPrivate
    {
        Point mouseCursorPos;
        Point mousePressPos;
        Point grabbedPos;
        Rect handleRect = {0, 0, 50, 50};
        Size moveArea;

        uint32_t range = 100;
        uint32_t value = 0;

        Orientation orientation = Vertical;
        bool enable = true;
        bool pressed = false;
        bool grabbed = false;

        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Color handleColors[AllStates];
        Color backgrounds[AllStates];

        std::map<int, std::function<void (uint32_t)> > scrollHandlers;
    };

    ScrollBar::ScrollBar(Orientation orientation, Widget *parent) : Widget(parent),
        d(new ScrollBarPrivate)
    {
        d->orientation = orientation;

        calcBorders({90, 90, 90, 1});
        calcHandeColors({50, 50, 50});
        setBackground({90, 90, 90});
        setCursorShape(Arrow);
    }

    ScrollBar::~ScrollBar()
    {
        delete d;
    }

    uint32_t ScrollBar::range()
    {
        return d->range;
    }

    void ScrollBar::setRange(uint32_t range)
    {
        if (range == d->range) return;
        if (range == 0) return;

        d->range = range;
        if (d->range > d->value) d->value = d->range;
        updateSizes();
        update();
    }

    uint32_t ScrollBar::value()
    {
        return d->value;
    }

    void ScrollBar::setValue(uint32_t value)
    {
        if (value == d->value) return;

        d->value = value;
        if (d->value > d->range) d->value = d->range;

        if (d->orientation == Vertical)
            d->handleRect.y = (d->moveArea.height * d->value) / d->range;
        else d->handleRect.x = (d->moveArea.width * d->value) / d->range;

        update();
    }

    void ScrollBar::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
        updateSizes();
    }

    void ScrollBar::setHandleColors(const Color &color, StyleState state)
    {
        if (state == AllStates) calcHandeColors(color);
        else d->handleColors[state] = color;
    }

    void ScrollBar::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->backgrounds[static_cast<StyleState>(i)] = color;
        else d->backgrounds[state] = color;
    }

    int ScrollBar::addScrollHandler(const std::function<void (uint32_t)> &func)
    {
        int id = static_cast<int>(d->scrollHandlers.size());
        for (auto const& it : d->scrollHandlers) if (it.first != ++id) break;
        d->scrollHandlers.insert({id, func});
        return id;
    }

    void ScrollBar::removeScrollHandler(int id)
    {
        auto it = d->scrollHandlers.find(id);
        if (it != d->scrollHandlers.end())
            d->scrollHandlers.erase(it);
    }

    void ScrollBar::paintEvent()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);

        setBrush(d->handleColors[d->styleState]);
        drawRect(d->handleRect);
    }

    void ScrollBar::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = state ? HoveredState : EnableState;
        update();
    }

    void ScrollBar::mouseMoveEvent(const Point &pos)
    {
        d->mouseCursorPos = pos;

        if (!d->grabbed) return;

        if (d->orientation == Vertical)
        {
            d->handleRect.y = pos.y - d->grabbedPos.y;

            if (d->handleRect.y < 0) d->handleRect.y = 0;
            else if(d->handleRect.y > d->moveArea.height)
                d->handleRect.y = d->moveArea.height;
        }
        else
        {
            d->handleRect.x = pos.x - d->grabbedPos.x;

            if (d->handleRect.x < 0) d->handleRect.x = 0;
            else if(d->handleRect.x > d->moveArea.width)
                d->handleRect.x = d->moveArea.width;
        }

        updateValue();
        update();
    }

    void ScrollBar::mouseButtonEvent(const MouseEvent &event)
    {
        if (event.button != ButtonLeft) return;
        d->pressed = event.pressed;

        if (event.pressed)
        {
            d->mousePressPos = d->mouseCursorPos;
            d->grabbed = contains(d->handleRect, d->mousePressPos);
            d->grabbedPos = { d->mousePressPos.x - d->handleRect.x,
                              d->mousePressPos.y - d->handleRect.y};
        }
        else d->grabbed = false;
    }

    void ScrollBar::resizeEvent(const Size &size)
    {
        std::ignore = size;
        updateSizes();
    }

    void ScrollBar::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(pen.color, d->borderPens[static_cast<StyleState>(i)].color, 6 * i);
    }

    void ScrollBar::calcHandeColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->handleColors[static_cast<StyleState>(i)], 2 * i);
    }

    void ScrollBar::updateValue()
    {
        uint32_t pos = d->handleRect.x;
        uint32_t dist = d->moveArea.width;

        if (d->orientation == Vertical)
        {
            pos = d->handleRect.y;
            dist = d->moveArea.height;
        }

        uint32_t value = (d->range * pos) / dist;
        if (value == d->value) return;

        d->value = value;
        for (const auto &it: d->scrollHandlers) it.second(d->value);
    }

    void ScrollBar::updateSizes()
    {
        if (d->orientation == Vertical)
        {
            d->handleRect.x = 0;
            d->handleRect.width = width() - d->borderPens[0].width;

            if (d->range > height() - MIN_HANDLE_SIZE) d->handleRect.height = MIN_HANDLE_SIZE;
            else d->handleRect.height = height() - d->range;

            d->moveArea.height = height() - d->handleRect.height - d->borderPens[0].width;
        }
        else
        {
            d->handleRect.y = 0;
            d->handleRect.height = height() - d->borderPens[0].width;

            if (d->range > height() - MIN_HANDLE_SIZE) d->handleRect.width = MIN_HANDLE_SIZE;
            else d->handleRect.width = width() - d->range;

            d->moveArea.width = width() - d->handleRect.width - d->borderPens[0].width;
        }
    }

} // namespace SA
