#include <iostream>
#include <algorithm>
#include <map>

#include "SACore/utility.h"
#include "SAGui/textedit.h"

namespace SA
{
    struct TextEdit::TextEditPrivate
    {
        std::vector<std::string> strings;

        int timerId = 0;

        int cursorX = 0;
        int cursorY = 0;
        int cursorRow = 0;
        int cursorColumn = 0;
        int cursorHeight = 10;
        int rowHeight = 10;

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

        d->strings.push_back(std::string());
        d->timerId = startTimer(500);
        d->cursorHeight = textHeight() + 2;
        d->rowHeight = textHeight() + 5;
    }

    TextEdit::~TextEdit()
    {
        killTimer(d->timerId);
        delete d;
    }

    void TextEdit::setText(const std::string &text)
    {
        int row = 0;
        for (const char &c: text)
        {
            if (c != '\n') d->strings[row].push_back(c);
            else { d->strings.push_back(std::string()); ++row; }
        }

        update();
    }

    std::string TextEdit::text()
    {
        std::string result;
        result.reserve(d->strings.size() * 50);

        for (const std::string &text: d->strings)
        { result += text; result += "\n"; }

        return std::move(result);
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


        int posX = textWidth(d->strings.at(d->cursorRow)) + 1;
        int posY = d->cursorRow * d->rowHeight;
//        drawRect(1, posY, posX, d->cursorHeight);




        brush = d->textColors[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);

        int row = -1;
        for (const std::string &text: d->strings)
            drawText(pen.width + 1, ++row * d->rowHeight, text);



        if (d->blinkState)
        {
            brush = d->textColors[d->styleState];
            setPen(brush.red, brush.green, brush.blue, 2);

            drawLine(posX, posY,
                     posX, posY + d->cursorHeight);
        }
    }

    void TextEdit::mouseMoveEvent(int x, int y)
    {
        d->cursorX = x;
        d->cursorY = y;
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
        if (!event.pressed) return;

        char symbol = getCharacter(event);

        if (symbol != 0) d->strings[d->cursorRow].push_back(symbol);
        else
        {
            if (event.keycode == Key_Backspace)
            {
                if(!d->strings[d->cursorRow].empty())
                    d->strings[d->cursorRow].pop_back();
            }
            else if (event.keycode == Key_Return)
            {

            }
        }

        update();
    }

    void TextEdit::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;
        if (!event.pressed) return;

        d->cursorRow = d->cursorY / d->rowHeight;

        if (d->cursorRow >= d->strings.size())
            d->cursorRow = d->strings.size() - 1;

        // Get char position

        update();
    }

    void TextEdit::focusEvent(bool state)
    {
        d->inFocus = state;
    }

    int TextEdit::calcCharPos()
    {
        if (d->cursorX <= 1) return 1;

        const std::string &text = d->strings.at(d->cursorRow);
        size_t size = textWidth(text);

        if (d->cursorX > size) return size + 1;

        int result = 0;
        int shift = size / 2;
        int length = text.size() - shift;
        for (size_t i=0; i<text.size(); ++i)
        {
            size = textWidth(text.data() + shift, length);

            if (d->cursorX > size)
            {

            }
            else
            {

            }



            if (size < 2) break;
        }

        return result;
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
