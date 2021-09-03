#include <iostream>
#include <algorithm>
#include <map>
#include "textedit.h"

namespace SA
{
    std::map<Keys, char> SYMBOLS_LOWERCASE =
    {
        { Key_0, '0' },
        { Key_1, '1' },
        { Key_2, '2' },
        { Key_3, '3' },
        { Key_4, '4' },
        { Key_5, '5' },
        { Key_6, '6' },
        { Key_7, '7' },
        { Key_8, '8' },
        { Key_9, '9' },
        { Key_A, 'a' },

        { Key_B, 'b' },
        { Key_C, 'c' },
        { Key_D, 'd' },
        { Key_E, 'e' },
        { Key_F, 'f' },
        { Key_G, 'g' },
        { Key_H, 'h' },
        { Key_I, 'i' },
        { Key_J, 'j' },
        { Key_K, 'k' },
        { Key_L, 'l' },
        { Key_M, 'm' },
        { Key_N, 'n' },
        { Key_O, 'o' },
        { Key_P, 'p' },
        { Key_Q, 'q' },
        { Key_R, 'r' },
        { Key_S, 's' },
        { Key_T, 't' },
        { Key_U, 'u' },
        { Key_V, 'v' },
        { Key_W, 'w' },
        { Key_X, 'x' },
        { Key_Y, 'y' },
        { Key_Z, 'z' }
    }; // SYMBOLS_MAP_US

    std::map<Keys, char> SYMBOLS_UPPERCASE =
    {
        { Key_0, '!' },
        { Key_1, '@' },
        { Key_2, '#' },
        { Key_3, '$' },
        { Key_4, '%' },
        { Key_5, '^' },
        { Key_6, '&' },
        { Key_7, '7' },
        { Key_8, '8' },
        { Key_9, '9' },
        { Key_A, 'a' },

        { Key_B, 'b' },
        { Key_C, 'c' },
        { Key_D, 'd' },
        { Key_E, 'e' },
        { Key_F, 'f' },
        { Key_G, 'g' },
        { Key_H, 'h' },
        { Key_I, 'i' },
        { Key_J, 'j' },
        { Key_K, 'k' },
        { Key_L, 'l' },
        { Key_M, 'm' },
        { Key_N, 'n' },
        { Key_O, 'o' },
        { Key_P, 'p' },
        { Key_Q, 'q' },
        { Key_R, 'r' },
        { Key_S, 's' },
        { Key_T, 't' },
        { Key_U, 'u' },
        { Key_V, 'v' },
        { Key_W, 'w' },
        { Key_X, 'x' },
        { Key_Y, 'y' },
        { Key_Z, 'z' }
    }; // SYMBOLS_MAP_US

    struct TextEdit::TextEditPrivate
    {
        std::string text;
        int timerId = 0;
        int cursorX = 0;
        int cursorY = 0;
        int cursorHeight = 10;
        bool blinkState = false;
        bool enable = true;
        bool inFocus = false;
        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Brush textColors[AllStates];
        Brush backgrounds[AllStates];

        std::map<int, std::function<void (bool)> > hoverHanders;
    };

    TextEdit::TextEdit(Widget *parent) : Widget(parent),
        d(new TextEditPrivate)
    {
        resize(150, 40);
        calcTextColors({20, 20, 20});
        calcBorders({90, 90, 90, 1});
        calcBackgrounds({250, 250, 250});

        d->cursorHeight = textHeight() + 2;
        d->timerId = startTimer(500);
    }

    TextEdit::~TextEdit()
    {
        killTimer(d->timerId);
        delete d;
    }

    void TextEdit::setText(const std::string &text)
    {
        d->text = text;
        update();
    }

    std::string TextEdit::text()
    {
        return std::move(d->text);
    }

    void TextEdit::setEnabled(bool state)
    {
        d->enable = state;
        d->styleState = state ? EnableState : DisableState;
        update();
    }

    bool TextEdit::isEnabled()
    {
        return d->enable;
    }

    void TextEdit::setTextColor(unsigned char red, unsigned char green,
                              unsigned char blue, StyleState state)
    {
        if (state == AllStates) calcTextColors({red, green, blue});
        else d->textColors[state] = {red, green, blue};
    }

    void TextEdit::setBorder(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned int width, StyleState state)
    {
        if (state == AllStates) calcBorders({red, green, blue, width});
        else d->borderPens[state] = {red, green, blue, width};
    }

    void TextEdit::setBackground(unsigned char red, unsigned char green,
                               unsigned char blue, StyleState state)
    {
        if (state == AllStates) calcBackgrounds({red, green, blue});
        else d->backgrounds[state] = {red, green, blue};
    }

    int TextEdit::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->hoverHanders)
            if (it.first != ++id) break;

        d->hoverHanders.insert({id, func});
        return id;
    }

    void TextEdit::removeHoverHandler(int id)
    {
        auto it = d->hoverHanders.find(id);
        if (it != d->hoverHanders.end())
            d->hoverHanders.erase(it);
    }

    void TextEdit::timerEvent(int id)
    {
        if (d->timerId != id) return;

        if (d->inFocus)
        {
            d->blinkState = !d->blinkState;
            update();
        }
        else if(d->blinkState)
        {
            d->blinkState = false;
            update();
        }
    }

    void TextEdit::paintEvent()
    {
        Brush brush = d->backgrounds[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);

        Pen pen = d->borderPens[d->styleState];
        setPen(pen.red, pen.green, pen.blue, pen.width);
        drawRect(0, 0, width() - 1, height() - 1);

        brush = d->textColors[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);

        drawText(1, 1, d->text);

        if (d->blinkState)
        {
            brush = d->textColors[d->styleState];
            setPen(brush.red, brush.green, brush.blue, 2);

            int posX = textWidth(d->text) + 2;
            drawLine(posX, 2, posX, d->cursorHeight);
        }
    }

    void TextEdit::mouseMoveEvent(int x, int y)
    {
//        std::cout << __PRETTY_FUNCTION__
//                  << ", x: " << x
//                  << ", y: " << y
//                  << std::endl;
    }

    void TextEdit::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = state ? HoveredState : EnableState;
        update();
        for (const auto &it: d->hoverHanders) it.second(state);
    }

    void TextEdit::keyboardEvent(const KeyEvent &event)
    {
//        std::cout << __PRETTY_FUNCTION__
//                  << " pressed:" << event.pressed
//                  << ", key: " << event.keycode
//                  << ", alt: " << event.modifiers.alt
//                  << ", shift: " << event.modifiers.shift
//                  << ", ctrl: " << event.modifiers.ctrl
//                  << ", capsLock: " << event.modifiers.capsLock
//                  << ", numLock: " << event.modifiers.numLock
//                  << ", super: " << event.modifiers.super
//                  << std::endl;

        if (!event.pressed) return;
        if (SYMBOLS_LOWERCASE.find(event.keycode) == SYMBOLS_LOWERCASE.end()) return;

        d->text.push_back(SYMBOLS_LOWERCASE[event.keycode]);
        update();
    }

    void TextEdit::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;
    }

    void TextEdit::focusEvent(bool state)
    {
        std::cout << __PRETTY_FUNCTION__ << " state:" << state << std::endl;
        d->inFocus = state;
    }

    void TextEdit::calcTextColors(const Brush &brush)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->textColors[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((brush.red - 6 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.green - 6 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.blue - 6 * i), 0, 255))
                };
        }
    }

    void TextEdit::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->borderPens[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((pen.red - 6 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((pen.green - 6 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((pen.blue - 6 * i), 0, 255)),
                    pen.width
                };
        }
    }

    void TextEdit::calcBackgrounds(const Brush &brush)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
        {
            d->backgrounds[static_cast<StyleState>(i)] = {
                    static_cast<unsigned char>(std::clamp((brush.red - 2 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.green - 2 * i), 0, 255)),
                    static_cast<unsigned char>(std::clamp((brush.blue - 2 * i), 0, 255))
                };
        }
    }

} // namespace SA
