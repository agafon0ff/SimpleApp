#pragma once
#include <functional>
#include "SAGui/widget.h"

namespace SA
{
    class Button : public Widget
    {
    public:
        Button(Widget *parent = nullptr);
        virtual ~Button();

        void setText(const std::string &text);
        std::string text();

        int addPressHandler(const std::function<void (bool)> &func);
        void removePressHandler(int id);

        int addHoverHandler(const std::function<void (bool)> &func);
        void removeHoverHandler(int id);

    private:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mousePressEvent(bool state, unsigned int button);

        struct ButtonPrivate;
        ButtonPrivate * const d;

    }; // class Button

} // namespace SA
