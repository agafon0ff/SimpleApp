#pragma once
#include <functional>
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

        void setTextColor(unsigned char red, unsigned char green,
                          unsigned char blue, StyleState state = AllStates);

        void setBorder(unsigned char red, unsigned char green, unsigned char blue,
                       unsigned int width, StyleState state = AllStates);

        void setBackground(unsigned char red, unsigned char green,
                           unsigned char blue, StyleState state = AllStates);

        int addHoverHandler(const std::function<void (bool)> &func);
        void removeHoverHandler(int id);

        int addPressHandler(const std::function<void (bool)> &func);
        void removePressHandler(int id);

        int addCheckHandler(const std::function<void (bool)> &func);
        void removeCheckHandler(int id);

    private:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseButtonEvent(const MouseEvent &event);

        void calcTextColors(const Brush &brush);
        void calcBorders(const Pen &pen);
        void calcBackgrounds(const Brush &brush);

        struct ButtonPrivate;
        ButtonPrivate * const d;

    }; // class Button

} // namespace SA
