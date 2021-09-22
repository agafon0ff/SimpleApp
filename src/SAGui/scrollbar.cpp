#include <algorithm>
#include <iostream>

#include "SACore/utility.h"
#include "scrollbar.h"

using std::cout;
using std::endl;

namespace SA
{

    struct ScrollBar::ScrollBarPrivate
    {
        Point mouseCursorPos;
        Point mousePressPos;

        Orientation orientation = Vertical;
        bool enable = true;
        bool inFocus = false;
        bool pressed = false;

        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Color handleColors[AllStates];
        Color backgrounds[AllStates];
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

    void ScrollBar::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
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

    void ScrollBar::paintEvent()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);

        setBrush(d->handleColors[d->styleState]);

        if (d->orientation == Vertical)
            drawRect(0, 20, width() - 1, 50);
        else drawRect(20, 0, 50, height() - 1);
    }

    void ScrollBar::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = state ? HoveredState : EnableState;
        update();
    }

    void ScrollBar::mouseMoveEvent(const Point &pos)
    {

    }

    void ScrollBar::mouseButtonEvent(const MouseEvent &event)
    {

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

} // namespace SA
