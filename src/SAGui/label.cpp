#include "SAGui/label.h"
#include "SACore/utility.h"

namespace SA
{
    static const int DEFAULT_TEXT_SHIFT = 3;

    struct Label::LabelPrivate
    {
        std::string text;
        bool enable = true;
        Point textShiftPos = {DEFAULT_TEXT_SHIFT, DEFAULT_TEXT_SHIFT};

        // Text style
        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Color textColors[AllStates];
        Color backgrounds[AllStates];

        Alignment textAlignH = SA::AlignLeft;
        Alignment textAlignV = SA::AlignVCenter;
    };

    Label::Label(Widget *parent) : Widget(parent),
        d(new LabelPrivate)
    {
        resize(150, 40);
        calcTextColors({240, 240, 240});
        calcBorders({40, 40, 40, 1});
        setBackground({40, 40, 40});
    }

    Label::Label(const std::string &text, Widget *parent) : Label(parent)
    {
        d->text = text;
        calcTextShiftPos();
    }

    Label::~Label()
    {
        delete d;
    }

    void Label::setText(const std::string &text)
    {
        d->text = text;
        calcTextShiftPos();
        update();
    }

    std::string Label::text() const
    {
        return d->text;
    }

    void Label::clear()
    {
        d->text.clear();
        update();
    }

    void Label::setEnabled(bool state)
    {
        d->enable = state;
        d->styleState = state ? EnableState : DisableState;
        update();
    }

    bool Label::isEnabled() const
    {
        return d->enable;
    }

    void Label::setTextAlignmentH(Alignment state)
    {
        d->textAlignH = state;
        calcTextShiftPos();
        update();
    }

    void Label::setTextAlignmentV(Alignment state)
    {
        d->textAlignV = state;
        calcTextShiftPos();
        update();
    }

    Alignment Label::textAlignmentH() const
    {
        return d->textAlignH;
    }

    Alignment Label::textAlignmentV() const
    {
        return d->textAlignV;
    }

    void Label::setTextColor(const Color &color, StyleState state)
    {
        if (state == AllStates) calcTextColors(color);
        else d->textColors[state] = color;
    }

    void Label::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
    }

    void Label::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->backgrounds[static_cast<StyleState>(i)] = color;
        else d->backgrounds[state] = color;
    }

    void Label::paintEvent()
    {
        drawBackground();
        drawTextString();
    }

    void Label::resizeEvent(const SA::Size &size)
    {
        std::ignore = size;
        calcTextShiftPos();
    }

    void Label::calcTextShiftPos()
    {
        if (d->textAlignH == SA::AlignLeft)
            d->textShiftPos.x = DEFAULT_TEXT_SHIFT;
        else if (d->textAlignH == SA::AlignRight)
            d->textShiftPos.x = width() - textWidth(d->text) - DEFAULT_TEXT_SHIFT;
        else d->textShiftPos.x = width() / 2 - textWidth(d->text) / 2;

        if (d->textAlignV == SA::AlignTop)
            d->textShiftPos.y = DEFAULT_TEXT_SHIFT;
        else if (d->textAlignV == SA::AlignBottom)
            d->textShiftPos.y = height() - textHeight() - DEFAULT_TEXT_SHIFT;
        else d->textShiftPos.y = height() / 2 - textHeight() / 2;
    }

    void Label::calcTextColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->textColors[static_cast<StyleState>(i)], (6 * i) - 6);
    }

    void Label::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            d->borderPens[static_cast<StyleState>(i)].color = pen.color;
    }

    void Label::drawBackground()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);
    }

    void Label::drawTextString()
    {
        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 2);
        drawText(d->textShiftPos.x, d->textShiftPos.y, d->text);
    }

} // namespace SA
