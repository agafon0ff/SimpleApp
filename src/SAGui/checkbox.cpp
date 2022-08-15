#include "checkbox.h"
#include "utility.h"

namespace SA
{
    static const int DEFAULT_TEXT_SHIFT = 3;
    static const int DEFAULT_BOX_INDENT = 5;

    struct CheckBox::CheckBoxPrivate
    {
        std::string text;
        bool enable = true;
        bool checked = false;
        Point textShiftPos = {DEFAULT_TEXT_SHIFT, DEFAULT_TEXT_SHIFT};
        Rect boxRect = {DEFAULT_BOX_INDENT, DEFAULT_BOX_INDENT,
                        DEFAULT_BOX_INDENT, DEFAULT_BOX_INDENT};

        // Text style
        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Pen boxBorderPens[AllStates];
        Color textColors[AllStates];
        Color backgrounds[AllStates];
        Color boxBackgrounds[AllStates];
        Color checkmarkColors[AllStates];

        Alignment textAlignH = SA::AlignLeft;
        Alignment textAlignV = SA::AlignVCenter;
    };

    CheckBox::CheckBox(Widget *parent) : Widget(parent),
        d(new CheckBoxPrivate)
    {
        resize(150, 40);
        calcTextColors({240, 240, 240});
        calcBorders({40, 40, 40, 1});
        calcBoxBorders({90, 90, 90, 1});
        calcBoxBackgrounds({30, 30, 30});
        calcCheckmarkColors({120, 120, 120});
        setBackground({40, 40, 40});
    }

    CheckBox::CheckBox(const std::string &text, Widget *parent) : CheckBox(parent)
    {
        d->text = text;
        calcCheckboxRect();
        calcTextShiftPos();
    }

    CheckBox::~CheckBox()
    {
        delete d;
    }

    void CheckBox::setText(const std::string &text)
    {
        d->text = text;
        calcCheckboxRect();
        calcTextShiftPos();
        update();
    }

    std::string CheckBox::text() const
    {
        return d->text;
    }

    void CheckBox::clear()
    {
        d->text.clear();
        update();
    }

    void CheckBox::setEnabled(bool state)
    {
        d->enable = state;
        d->styleState = state ? EnableState : DisableState;
        update();
    }

    bool CheckBox::isEnabled() const
    {
        return d->enable;
    }

    void CheckBox::setChecked(bool state)
    {
        d->checked = state;
        update();
    }

    bool CheckBox::isChecked()
    {
        return d->checked;
    }

    void CheckBox::setTextAlignmentH(Alignment state)
    {
        d->textAlignH = state;
        calcCheckboxRect();
        calcTextShiftPos();
        update();
    }

    void CheckBox::setTextAlignmentV(Alignment state)
    {
        d->textAlignV = state;
        calcCheckboxRect();
        calcTextShiftPos();
        update();
    }

    Alignment CheckBox::textAlignmentH() const
    {
        return d->textAlignH;
    }

    Alignment CheckBox::textAlignmentV() const
    {
        return d->textAlignV;
    }

    void CheckBox::setTextColor(const Color &color, StyleState state)
    {
        if (state == AllStates) calcTextColors(color);
        else d->textColors[state] = color;
    }

    void CheckBox::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
    }

    void CheckBox::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->backgrounds[static_cast<StyleState>(i)] = color;
        else d->backgrounds[state] = color;
    }

    void CheckBox::setBoxBorderPen(const Pen &pen, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->boxBorderPens[static_cast<StyleState>(i)] = pen;
        else d->boxBorderPens[state] = pen;
    }

    void CheckBox::setBoxBackground(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->boxBackgrounds[static_cast<StyleState>(i)] = color;
        else d->boxBackgrounds[state] = color;
    }

    void CheckBox::setCheckmarkColor(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->checkmarkColors[static_cast<StyleState>(i)] = color;
        else d->checkmarkColors[state] = color;
    }

    void CheckBox::paintEvent()
    {
        drawBackground();
        drawTextString();
        drawCheckmarkBox();
    }

    void CheckBox::mouseHoverEvent(bool state)
    {

    }

    void CheckBox::mouseButtonEvent(const MouseEvent &event)
    {

    }

    void CheckBox::resizeEvent(const SA::Size &size)
    {
        std::ignore = size;
        calcCheckboxRect();
        calcTextShiftPos();
    }

    void CheckBox::calcTextShiftPos()
    {
        if (d->textAlignH == SA::AlignLeft)
            d->textShiftPos.x = d->boxRect.x + d->boxRect.width + DEFAULT_TEXT_SHIFT * 2;
        else if (d->textAlignH == SA::AlignRight)
            d->textShiftPos.x = width() - textWidth(d->text) - d->boxRect.width - DEFAULT_TEXT_SHIFT * 2;
        else d->textShiftPos.x = width() / 2 - textWidth(d->text) / 2;

        if (d->textAlignV == SA::AlignTop)
            d->textShiftPos.y = DEFAULT_TEXT_SHIFT;
        else if (d->textAlignV == SA::AlignBottom)
            d->textShiftPos.y = height() - textHeight() - DEFAULT_TEXT_SHIFT;
        else d->textShiftPos.y = height() / 2 - textHeight() / 2;
    }

    void CheckBox::calcCheckboxRect()
    {
        int indent = DEFAULT_BOX_INDENT;
        uint32_t boxWidth = static_cast<uint32_t>(height() - (indent * 2) - 1);
        if (d->textAlignH == SA::AlignLeft)
            d->boxRect = {0, indent, boxWidth, boxWidth};
        else d->boxRect = {width() - static_cast<int32_t>(boxWidth), indent, boxWidth, boxWidth};
    }

    void CheckBox::calcTextColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->textColors[static_cast<StyleState>(i)], (6 * i) - 6);
    }

    void CheckBox::calcBoxBackgrounds(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->boxBackgrounds[static_cast<StyleState>(i)], (6 * i) - 6);
    }

    void CheckBox::calcCheckmarkColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            d->checkmarkColors[static_cast<StyleState>(i)] = color;
    }

    void CheckBox::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            d->borderPens[static_cast<StyleState>(i)] = pen;
    }

    void CheckBox::calcBoxBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            darker(pen.color, d->boxBorderPens[static_cast<StyleState>(i)].color, 6 * i);
            d->boxBorderPens[static_cast<StyleState>(i)].width = pen.width;
        }
    }

    void CheckBox::drawBackground()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);
    }

    void CheckBox::drawTextString()
    {
        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 2);
        drawText(d->textShiftPos.x, d->textShiftPos.y, d->text);
    }

    void CheckBox::drawCheckmarkBox()
    {
        setBrush(d->boxBackgrounds[d->styleState]);
        setPen(d->boxBorderPens[d->styleState]);
        drawRect(d->boxRect);

        setBrush(d->checkmarkColors[d->styleState]);
        setPen(d->checkmarkColors[d->styleState], 1);
        drawRect(d->boxRect.x + 3, d->boxRect.y + 3,
                 d->boxRect.width - 6, d->boxRect.height - 6);
    }

} // namespace SA
