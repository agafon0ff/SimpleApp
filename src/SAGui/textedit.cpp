#include <iostream>
#include <algorithm>
#include <map>

#include "SAGui/textedit.h"
#include "SACore/utility.h"

using std::cout;
using std::endl;

namespace SA
{

    struct TextSelection
    {
        bool selected = false;

        uint32_t posStart = 0;
        uint32_t posEnd = 0;

        uint32_t rowStart = 0;
        uint32_t rowEnd = 0;

        uint32_t columnStart = 0;
        uint32_t columnEnd = 0;
    };

    struct TextEdit::TextEditPrivate
    {
        std::vector<std::string> strings;

        int timerId = 0;

        Point cursorPos;
        Point pressPos;

        uint32_t currentRow = 0;
        uint32_t currentColumn = 0;

        uint32_t textCursorX = 0;
        uint32_t cursorHeight = 10;
        uint32_t rowHeight = 10;

        bool blinkState = false;
        bool enable = true;
        bool inFocus = false;
        bool pressed = false;

        // Text selection
        TextSelection selection;

        // Text style
        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Color textColors[AllStates];
        Color backgrounds[AllStates];
        Color selectionColor = {180, 180, 180};

        // Events listeners
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
        d->cursorHeight = textHeight() + 6;
        d->rowHeight = textHeight() + 5;

        setCursorShape(Text);
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

    bool TextEdit::isTextSelected()
    {
        return d->selection.selected;
    }

    std::string TextEdit::selectedText()
    {
        return std::string();
    }

    void TextEdit::removeSelectedText()
    {
        if (!d->selection.selected) return;

        if (d->selection.rowStart == d->selection.rowEnd)
        {
            const int posX = std::min(d->selection.columnStart, d->selection.columnEnd);
            const int posY = std::max(d->selection.columnStart, d->selection.columnEnd) - posX;
            d->strings[d->currentRow].erase(posX, posY);
            d->textCursorX = std::min(d->selection.posStart, d->selection.posEnd);
            d->currentColumn = posX;
            update();
        }
        else
        {
            TextSelection selection = d->selection;

            if (d->selection.rowStart > d->selection.rowEnd)
            {
                std::swap(selection.columnStart, selection.columnEnd);
                std::swap(selection.posStart, selection.posEnd);
                std::swap(selection.rowStart, selection.rowEnd);
            }

            const int eraseSize = d->strings.at(selection.rowStart).size() - selection.columnStart;
            d->strings[selection.rowStart].erase(selection.columnStart, eraseSize);
            d->strings[selection.rowEnd].erase(0, selection.columnEnd);
            d->strings[selection.rowStart].append(d->strings.at(selection.rowEnd));

            const int rowMin = std::min(d->selection.rowStart, d->selection.rowEnd) + 1;
            const int rowMax = std::max(d->selection.rowStart, d->selection.rowEnd) + 1;
            for (int i=rowMin; i<rowMax; ++i) d->strings.erase(d->strings.begin() + i);

            d->textCursorX = selection.posStart;
            d->currentRow = selection.rowStart;
            d->currentColumn = selection.columnStart;

            update();
        }

        d->selection.selected = false;
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

    void TextEdit::setTextColor(const Color &color, StyleState state)
    {
        if (state == AllStates) calcTextColors(color);
        else d->textColors[state] = color;
    }

    void TextEdit::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
    }

    void TextEdit::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates) calcBackgrounds(color);
        else d->backgrounds[state] = color;
    }

    void TextEdit::setSelectionColor(const Color &color)
    {
        d->selectionColor = color;
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
        drawBackground();
        drawTextSelection();
        drawTextStrings();
        drawTextCursor();
    }

    void TextEdit::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = state ? HoveredState : EnableState;
        update();
        for (const auto &it: d->hoverHanders) it.second(state);
    }

    void TextEdit::mouseMoveEvent(const Point &pos)
    {
        d->cursorPos = pos;

        if (d->pressed)
        {
            calcCurrentRow();
            calcTextCursorPos();
            updateTextSelection();
            update();
        }
    }

    void TextEdit::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;

        d->pressed = event.pressed;

        if (event.pressed)
        {
            d->pressPos = d->cursorPos;

            calcCurrentRow();
            calcTextCursorPos();
            updateTextSelection(true);
            update();
        }
        else
        {
            if (d->pressPos == d->cursorPos)
                d->selection.selected = false;
        }
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
            case Key_Backspace: keyReactionBackspace(); break;
            case Key_Delete:    keyReactionDelete(); break;
            case Key_Return:    keyReactionReturn(); break;
            case Key_Left:      moveTextCursor(Left); break;
            case Key_Right:     moveTextCursor(Right); break;
            case Key_Up:        moveTextCursor(Up); break;
            case Key_Down:      moveTextCursor(Down); break;
            case Key_Home:      keyReactionHome(); break;
            case Key_End:       keyReactionEnd(); break;
            case Key_Tab:       keyReactionTab(); break;
            default: break;
            }
        }

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

    void TextEdit::updateTextSelection(bool justPressed)
    {
        if (justPressed)
        {
            d->selection.posStart = d->textCursorX;
            d->selection.rowStart = d->currentRow;
            d->selection.columnStart = d->currentColumn;
        }

        d->selection.posEnd = d->textCursorX;
        d->selection.rowEnd = d->currentRow;
        d->selection.columnEnd = d->currentColumn;


        d->selection.selected = (d->selection.posStart != d->selection.posEnd ||  d->selection.rowStart != d->selection.rowEnd);
    }

    void TextEdit::keyReactionBackspace()
    {
        if (isTextSelected())
        {
            removeSelectedText();
        }
        else
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
        }
    }

    void TextEdit::keyReactionDelete()
    {
        if (isTextSelected())
        {
            removeSelectedText();
        }
        else
        {
            const std::string &text = d->strings.at(d->currentRow);

            if ((text.size() - d->currentColumn) > 0)
            {
                d->strings[d->currentRow].erase(d->currentColumn, 1);
            }
            else if(d->strings.size() > d->currentRow + 1)
            {
                d->strings[d->currentRow].append(d->strings.at(d->currentRow + 1));
                d->strings.erase(d->strings.begin() + d->currentRow + 1);
            }
        }
    }

    void TextEdit::keyReactionReturn()
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
    }

    void TextEdit::keyReactionHome()
    {
        d->currentColumn = 0;
        d->textCursorX = 1;
        d->blinkState = true;
    }

    void TextEdit::keyReactionEnd()
    {
        d->currentColumn = d->strings.at(d->currentRow).size();
        d->textCursorX = textWidth(d->strings.at(d->currentRow));
        d->blinkState = true;
    }

    void TextEdit::keyReactionTab()
    {
        d->strings[d->currentRow].insert(d->currentColumn, 4, ' ');
        d->currentColumn += 3;
        moveTextCursor(Right);
    }

    void TextEdit::calcCurrentRow()
    {
        d->currentRow = std::max(d->cursorPos.y, 0) / d->rowHeight;

        if (d->currentRow >= d->strings.size())
            d->currentRow = d->strings.size() - 1;
    }

    void TextEdit::calcTextCursorPos()
    {
        int delta = 3;

        if (d->cursorPos.x <= delta)
        {
            d->currentColumn = 0;
            d->textCursorX = 1;
            return;
        }

        const std::string &text = d->strings.at(d->currentRow);
        size_t result = textWidth(text);

        if (d->cursorPos.x > result)
        {
            d->currentColumn = text.size();
            d->textCursorX = result;
            return;
        }

        int length = text.size();
        int half = text.size() / 2;

        for (size_t i=0; i<text.size(); ++i)
        {
            if (d->cursorPos.x > (result + delta)) length += half;
            else length -= half;

            if (half > 1) half = half / 2;

            result = textWidth(text.data(), length);
            if (std::abs(d->cursorPos.x - static_cast<int>(result)) <= delta) break;
        }

        d->currentColumn = length;
        d->textCursorX = result;
    }

    void TextEdit::calcTextColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->textColors[static_cast<StyleState>(i)], 6 * i);
    }

    void TextEdit::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(pen.color, d->borderPens[static_cast<StyleState>(i)].color, 6 * i);
    }

    void TextEdit::calcBackgrounds(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->backgrounds[static_cast<StyleState>(i)], 2 * i);
    }

    void TextEdit::drawBackground()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);
    }

    void TextEdit::drawTextSelection()
    {
        if (!d->selection.selected) return;

        setBrush(d->selectionColor);
        setPen(d->selectionColor, 1);

        if (d->selection.rowStart == d->selection.rowEnd) // one line
        {
            const int posX = std::min(d->selection.posStart, d->selection.posEnd);
            const int posY = std::max(d->selection.posStart, d->selection.posEnd) - posX;
            drawRect(posX, d->selection.rowStart * d->rowHeight, posY, d->rowHeight);
        }
        else
        {
            uint32_t posStart = d->selection.posStart;
            uint32_t posEnd = d->selection.posEnd;
            uint32_t rowStart = d->selection.rowStart;
            uint32_t rowEnd = d->selection.rowEnd;

            if (d->selection.rowStart > d->selection.rowEnd){ std::swap(posStart, posEnd); std::swap(rowStart, rowEnd);}
            drawRect(0, rowEnd * d->rowHeight, posEnd, d->rowHeight);
            drawRect(posStart, rowStart * d->rowHeight, textWidth(d->strings.at(rowStart)) - posStart, d->rowHeight);

            const int rowMin = std::min(d->selection.rowStart, d->selection.rowEnd) + 1;
            const int rowMax = std::max(d->selection.rowStart, d->selection.rowEnd);
            for (int i=rowMin; i<rowMax; ++i) drawRect(0, i*d->rowHeight, textWidth(d->strings.at(i)), d->rowHeight);
        }

    }

    void TextEdit::drawTextStrings()
    {
        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 2);

        int row = -1;
        for (const std::string &text: d->strings)
            drawText(0, ++row * d->rowHeight, text);
    }

    void TextEdit::drawTextCursor()
    {
        if (!d->blinkState) return;

        setPen(d->textColors[d->styleState], 2);
        const int posY = d->currentRow * d->rowHeight - 2;
        drawLine(d->textCursorX, posY,
                 d->textCursorX, posY + d->cursorHeight);
    }

} // namespace SA
