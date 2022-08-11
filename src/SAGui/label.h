#pragma once
#include <functional>
#include "widget.h"
#include "global.h"

namespace SA
{
    class Label : public Widget
    {
    public:
        Label(Widget *parent = nullptr);
        Label(const std::string &text, Widget *parent = nullptr);
        ~Label();

        void setText(const std::string &text);
        std::string text() const;
        void clear();

        void setEnabled(bool state);
        bool isEnabled() const;

        void setTextAlignmentH(SA::Alignment state);
        void setTextAlignmentV(SA::Alignment state);
        SA::Alignment textAlignmentH() const;
        SA::Alignment textAlignmentV() const;

        void setTextColor(const Color &color, StyleState state = AllStates);
        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);
        void setSelectionColor(const Color &color);

    protected:
        virtual void paintEvent();
        virtual void resizeEvent(const SA::Size &size);

    private:
        void calcTextShiftPos();
        void calcTextColors(const Color &color);
        void calcBorders(const Pen &pen);

        void drawBackground();
        void drawTextString();

        struct LabelPrivate;
        LabelPrivate * const d;
    }; // class Label
} // namespace SA
