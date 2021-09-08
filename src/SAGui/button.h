#pragma once
#include <functional>
#include <iostream>
#include "SAGui/widget.h"

namespace SA
{
    class Button : public Widget
    {
    public:

        enum StyleState
        {
            DisableState,
            EnableState,
            HoveredState,
            PressedState,
            CheckedState,
            AllStates
        };

        explicit Button(Widget *parent = nullptr);
        Button(const std::string &text, Widget *parent = nullptr);
        virtual ~Button();

        void setText(const std::string &text);
        std::string text();

        void setEnabled(bool state);
        bool isEnabled();

        void setCheckable(bool state);
        bool isCheckable();

        void setChecked(bool state);
        bool isChecked();
        bool isPressed();

        void setTextColor(const Color &color, StyleState state = AllStates);
        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);

        int addHoverHandler(const std::function<void (bool)> &func);
        void removeHoverHandler(int id);

        int addPressHandler(const std::function<void (bool)> &func);
        void removePressHandler(int id);

        int addCheckHandler(const std::function<void (bool)> &func);
        void removeCheckHandler(int id);

    protected:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseButtonEvent(const MouseEvent &event);

    private:
        void calcTextColors(const Color &brush);
        void calcBorders(const Pen &pen);
        void calcBackgrounds(const Color &brush);

        struct ButtonPrivate;
        ButtonPrivate * const d;

    }; // class Button

} // namespace SA
