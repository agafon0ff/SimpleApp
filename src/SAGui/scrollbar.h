#pragma once
#include <functional>
#include "widget.h"

namespace SA
{
    class ScrollBar : public Widget
    {
    public:
        explicit ScrollBar(Orientation orientation, Widget *parent = nullptr);
        virtual ~ScrollBar();

        uint32_t range();
        void setRange(uint32_t range);

        uint32_t value();
        void setValue(uint32_t value);

        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setHandleColors(const Color &color, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);

        int addScrollHandler(const std::function<void (uint32_t)> &func);
        void removeScrollHandler(int id);

    protected:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseMoveEvent(const Point &pos);
        virtual void mouseButtonEvent(const MouseEvent &event);
        virtual void resizeEvent(const Size &size);

    private:

        void calcBorders(const Pen &pen);
        void calcHandeColors(const Color &color);

        void updateValue();
        void updateSizes();

        struct ScrollBarPrivate;
        ScrollBarPrivate * const d;

    }; // class ScrollBar
} // namespace SA

