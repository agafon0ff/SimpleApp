#pragma once
#include <functional>
#include "SAGui/widget.h"

namespace SA
{
    class TextEdit : public Widget
    {
    public:

        enum StyleState
        {
            DisableState,
            EnableState,
            HoveredState,
            AllStates
        };

        explicit TextEdit(Widget *parent = nullptr);
        virtual ~TextEdit();

        void setText(const std::string &text);
        std::string text();

        void setEnabled(bool state);
        bool isEnabled();

        void setTextColor(unsigned char red, unsigned char green,
                          unsigned char blue, StyleState state = AllStates);

        void setBorder(unsigned char red, unsigned char green, unsigned char blue,
                       unsigned int width, StyleState state = AllStates);

        void setBackground(unsigned char red, unsigned char green,
                           unsigned char blue, StyleState state = AllStates);

        int addHoverHandler(const std::function<void (bool)> &func);
        void removeHoverHandler(int id);

    private:
        virtual void timerEvent(int id);
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mousePressEvent(bool state, unsigned int button);
        virtual void focusEvent(bool state);

        void calcTextColors(const Brush &brush);
        void calcBorders(const Pen &pen);
        void calcBackgrounds(const Brush &brush);

        struct TextEditPrivate;
        TextEditPrivate * const d;

    }; // TextEdit

} // namespace SA
