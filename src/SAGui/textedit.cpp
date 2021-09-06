#include <iostream>
#include <algorithm>
#include <map>

#include "SACore/utility.h"
#include "SAGui/textedit.h"

using std::cout;
using std::endl;

namespace SA
{
    struct TextEdit::TextEditPrivate
    {
        std::vector<std::string> strings;

        int timerId = 0;

        int cursorX = 0;
        int cursorY = 0;
        int currentRow = 0;
        int currentColumn = 0;
        int textCursorX = 0;
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

        brush = d->textColors[d->styleState];
        setBrush(brush.red, brush.green, brush.blue);

        int row = -1;
        for (const std::string &text: d->strings)
            drawText(0, ++row * d->rowHeight, text);

        if (d->blinkState)
        {
            brush = d->textColors[d->styleState];
            setPen(brush.red, brush.green, brush.blue, 2);

            int posY = d->currentRow * d->rowHeight;
            drawLine(d->textCursorX, posY,
                     d->textCursorX, posY + d->cursorHeight);
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

        if (symbol != 0)
        {
            d->strings[d->currentRow].insert(d->currentColumn, 1, symbol);
            moveTextCursor(Right);
        }
        else
        {
            switch (event.keycode)
            {
            case Key_Backspace:
            {
                if(d->currentColumn > 0)
                {
                    moveTextCursor(Left);
                    d->strings[d->currentRow].erase(d->currentColumn, 1);
                }
                else if(d->currentRow > 0)
                {
                    const std::string &text = d->strings.at(d->currentRow);
                    moveTextCursor(Left);
                    d->strings[d->currentRow].append(text);
                    d->strings.erase(d->strings.begin() + d->currentRow + 1);
                }
                break;
            }
            case Key_Delete:
            {
                if(!d->strings.at(d->currentRow).empty())
                    d->strings[d->currentRow].erase(d->currentColumn, 1);
                break;
            }
            case Key_Return:
            {
                if (d->currentColumn >= d->strings.at(d->currentRow).size())
                {
                    ++d->currentRow;
                    d->strings.insert(d->strings.begin() + d->currentRow, std::string());
                }
                else
                {
                    const std::string &text = d->strings.at(d->currentRow);
                    ++d->currentRow;
                    d->strings.insert(d->strings.begin() + d->currentRow, text.substr(d->currentColumn));
                    d->strings[d->currentRow - 1].erase(d->currentColumn, text.size() - d->currentColumn);

                }
                d->currentColumn = 0;
                d->textCursorX = 1;

                break;
            }
            case Key_Left: moveTextCursor(Left); break;
            case Key_Right: moveTextCursor(Right); break;
            case Key_Up: moveTextCursor(Up); break;
            case Key_Down: moveTextCursor(Down); break;
            case Key_Home:
            {
                d->currentColumn = 0;
                d->textCursorX = 1;
                d->blinkState = true;
                break;
            }
            case Key_End:
            {
                d->currentColumn = d->strings.at(d->currentRow).size();
                d->textCursorX = textWidth(d->strings.at(d->currentRow));
                d->blinkState = true;
                break;
            }

            default: break;
            }
        }

        update();
    }

    void TextEdit::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;
        if (!event.pressed) return;

        d->currentRow = d->cursorY / d->rowHeight;

        if (d->currentRow >= d->strings.size())
            d->currentRow = d->strings.size() - 1;

        calcTextCursorPos();
        update();
    }

    void TextEdit::focusEvent(bool state)
    {
        d->inFocus = state;
    }

    void TextEdit::moveTextCursor(Direction dir)
    {
        switch (dir)
        {
        case Left:
        {
            --d->currentColumn;

            if (d->currentColumn < 0)
            {
                if (d->currentRow > 0)
                {
                    --d->currentRow;
                    d->currentColumn = d->strings.at(d->currentRow).size();
                }
                else d->currentColumn = 0;
            }

            break;
        }
        case Right:
        {
            ++d->currentColumn;

            const std::string &text = d->strings.at(d->currentRow);

            if (d->currentColumn > text.size())
            {
                if (d->strings.size() - 1 > d->currentRow)
                {
                    ++d->currentRow;
                    d->currentColumn = 0;
                }
                else d->currentColumn = text.size();
            }

            break;
        }
        case Up:
        {

            --d->currentRow;

            if (d->currentRow < 0) d->currentRow = 0;
            else
            {
                const std::string &text = d->strings.at(d->currentRow);
                if (d->currentColumn > text.size()) d->currentColumn = text.size();
            }

            break;
        }
        case Down:
        {
            ++d->currentRow;

            if (d->currentRow >= d->strings.size())
                d->currentRow = d->strings.size() - 1;
            else
            {
                const std::string &text = d->strings.at(d->currentRow);
                if (d->currentColumn > text.size()) d->currentColumn = text.size();
            }
            break;
        }
        default: break;
        }

        d->textCursorX = textWidth(d->strings.at(d->currentRow).data(), d->currentColumn);
        d->blinkState = true;
        update();
    }

    void TextEdit::calcTextCursorPos()
    {
        int delta = 3;

        if (d->cursorX <= delta)
        {
            d->currentColumn = 0;
            d->textCursorX = 1;
            return;
        }

        const std::string &text = d->strings.at(d->currentRow);
        size_t result = textWidth(text);

        if (d->cursorX > result)
        {
            d->currentColumn = text.size();
            d->textCursorX = result;
            return;
        }

        int length = text.size();
        int half = text.size() / 2;

        for (size_t i=0; i<text.size(); ++i)
        {
            if (d->cursorX > (result + delta)) length += half;
            else length -= half;

            if (half > 1) half = half / 2;

            result = textWidth(text.data(), length);
            if (abs(d->cursorX - result) <= delta) break;
        }

        d->currentColumn = length;
        d->textCursorX = result;
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
