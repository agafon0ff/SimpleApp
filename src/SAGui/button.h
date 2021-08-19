#pragma once
#include "SAGui/widget.h"

namespace SA
{
    class Button : public Widget
    {
    public:
        Button(Widget *parent = nullptr);
        virtual ~Button();

    private:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);

        struct ButtonPrivate;
        ButtonPrivate * const d;

    }; // class Button

} // namespace SA
