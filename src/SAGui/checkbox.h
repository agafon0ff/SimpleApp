#pragma once
#include <functional>
#include "widget.h"
#include "global.h"

namespace SA
{
    class CheckBox : public Widget
    {
    public:
        CheckBox(Widget *parent = nullptr);
        CheckBox(const std::string &text, Widget *parent = nullptr);
        ~CheckBox();

        void setText(const std::string &text);
        std::string text() const;
        void clear();

        void setEnabled(bool state);
        bool isEnabled() const;

        void setChecked(bool state);
        bool isChecked();

        void setTextAlignmentH(SA::Alignment state);
        void setTextAlignmentV(SA::Alignment state);
        SA::Alignment textAlignmentH() const;
        SA::Alignment textAlignmentV() const;

        void setTextColor(const Color &color, StyleState state = AllStates);
        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);
        void setSelectionColor(const Color &color);

        void setBoxBorderPen(const Pen &pen, StyleState state = AllStates);
        void setCheckmarkColor(const Color &color, StyleState state = AllStates);
        void setBoxBackground(const Color &color, StyleState state = AllStates);

    protected:
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseButtonEvent(const MouseEvent &event);
        virtual void resizeEvent(const SA::Size &size);

    private:
        void calcTextShiftPos();
        void calcCheckboxRect();
        void calcTextColors(const Color &color);
        void calcBoxBackgrounds(const Color &color);
        void calcCheckmarkColors(const Color &color);
        void calcBorders(const Pen &pen);
        void calcBoxBorders(const Pen &pen);

        void drawBackground();
        void drawTextString();
        void drawCheckmarkBox();

        struct CheckBoxPrivate;
        CheckBoxPrivate * const d;
    }; // class CheckBox
} // namespace SA

