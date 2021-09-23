#pragma once
#include <functional>
#include "SAGui/widget.h"

namespace SA
{
    class ScrollBar : public Widget
    {
    public:
        explicit ScrollBar(Orientation orientation, Widget *parent = nullptr);
        virtual ~ScrollBar();


        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setHandleColors(const Color &color, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);

    protected:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseMoveEvent(const Point &pos);
        virtual void mouseButtonEvent(const MouseEvent &event);
        virtual void resizeEvent(const Size &size);

    private:

        void calcBorders(const Pen &pen);
        void calcHandeColors(const Color &color);

        void updateSizes();

        struct ScrollBarPrivate;
        ScrollBarPrivate * const d;

    }; // class ScrollBar
} // namespace SA

