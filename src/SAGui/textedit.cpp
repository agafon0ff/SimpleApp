#include <algorithm>
#include <iostream>
#include <stack>
#include <map>

#include "SAGui/textedit.h"
#include "SAGui/clipboard.h"
#include "SACore/utility.h"

using std::cout;
using std::endl;

namespace SA
{
    struct SA::TextEdit::TextEditPrivate
    {
        std::vector<std::string> strings;
        std::vector<size_t> rowsWidths;
        std::stack<SA::TextAction> actions;

        int timerId = 0;

        Point textCursorPos;
        Point mouseCursorPos;
        Point mousePressPos;
        Point textShiftPos = {3, 3};
        Size  textAreaSize = {0, 0};

        uint32_t currentRow = 0;
        uint32_t currentColumn = 0;

        uint32_t cursorHeight = 10;
        uint32_t rowHeight = 10;
        size_t   textLength = 0;
        int32_t  textIndent[sizeof(Side)] = {3, 3, 18, 18};
        int16_t  scrollRate = 20;
        int32_t  scrollBarWidth = 12;

        bool blinkState = false;
        bool enable = true;
        bool inFocus = false;
        bool pressed = false;

        // Text selection
        SA::TextSelection selection;

        // Text style
        StyleState styleState = EnableState;
        Pen borderPens[AllStates];
        Color textColors[AllStates];
        Color backgrounds[AllStates];
        Color selectionColor = {140, 140, 140};

        // Events listeners
        std::map<int, std::function<void (bool)> > hoverHanders;

        SA::ScrollBar *scrollBarV = nullptr;
        SA::ScrollBar *scrollBarH = nullptr;
    };

    TextEdit::TextEdit(Widget *parent) : SA::Widget(parent),
        d(new TextEditPrivate)
    {
        using namespace std::placeholders;

        d->scrollBarV = new SA::ScrollBar(Vertical, this);
        d->scrollBarV->addScrollHandler(std::bind(&TextEdit::onScrollVertical, this, _1));

        d->scrollBarH = new SA::ScrollBar(Horizontal, this);
        d->scrollBarH->addScrollHandler(std::bind(&TextEdit::onScrollHorizontal, this, _1));

        d->timerId = startTimer(500);
        d->rowHeight = textHeight() + 5;
        d->cursorHeight = d->rowHeight;

        resize(150, 40);
        setCursorShape(Text);
        calcTextColors({240, 240, 240});
        calcBorders({90, 90, 90, 1});
        setBackground({30, 30, 30});
    }

    TextEdit::~TextEdit()
    {
        killTimer(d->timerId);

        delete d->scrollBarV;
        delete d->scrollBarH;
        delete d;
    }

    void TextEdit::setText(const std::string &text)
    {
        clear();
        append(text);
    }

    void TextEdit::append(char symbol)
    {
        d->strings.push_back(std::string(1, symbol));
        d->textLength += 2;
        calcTextAreaSize(d->strings.size() - 1);
        calcScrollBars();
        update();
    }

    void TextEdit::append(const std::string &text)
    {
        size_t pos = 0, step = 0, size = 0;
        size_t psize = d->strings.size();

        for (const char &c: text)
        {
            ++step;
            if (c == '\n')
            {
                d->strings.push_back(text.substr(pos, size));
                pos = step;
                size = 0;
            }
            else ++size;
        }

        d->strings.push_back(text.substr(pos, size));
        d->textLength += text.size();
        calcTextAreaSize(psize, d->strings.size() - psize);
        calcScrollBars();
        update();
    }

    void TextEdit::insert(uint64_t pos, char symbol)
    {
        uint32_t row = 0, column = 0;
        calcRowColumn(pos, row, column);
        insert(row, column, symbol);
    }

    void TextEdit::insert(uint32_t row, uint32_t column, char symbol)
    {
        if (d->strings.empty())
            d->strings.push_back(std::string());

        if (row >= d->strings.size())
            row = d->strings.size() - 1;

        size_t size = d->strings.at(row).size();
        if (column > size) column = size;

        if (symbol == '\n')
        {
            d->strings.insert(d->strings.begin() + row + 1, d->strings[row].substr(column, size-column));
            d->strings[row].erase(column, size-column);
            calcScrollBars();
        }
        else d->strings[row].insert(column, 1, symbol);

        ++d->textLength;
        calcTextAreaSize(row);
        update();
    }

    void TextEdit::insert(uint64_t pos, const std::string &text)
    {
        uint32_t row = 0, column = 0;
        calcRowColumn(pos, row, column);
        insert(row, column, text);
    }

    void TextEdit::insert(uint32_t row, uint32_t column, const std::string &text)
    {
        if (d->strings.empty())
            d->strings.push_back(std::string());

        if (row >= d->strings.size())
            row = d->strings.size() - 1;

        size_t psize = row;
        size_t size = d->strings.at(row).size();
        if (column > size) column = size;

        std::string remainder = d->strings.at(row).substr(column, size - column);
        d->strings[row].erase(column, size - column);

        size_t pos = 0, step = 0; size = 0;
        for (const char &c: text)
        {
            ++step;
            if (c == '\n')
            {
                if (pos == 0) d->strings[row].append(text.substr(pos, size));
                else d->strings.insert(d->strings.begin() + row, text.substr(pos, size));
                pos = step;
                size = 0;
                ++row;
            }
            else ++size;
        }

        if (pos == 0) d->strings[row].append(text.substr(pos, size));
        else d->strings.insert(d->strings.begin() + row, text.substr(pos, size));

        d->strings[row].append(remainder);
        d->textLength += text.size();
        calcTextAreaSize(psize, row - psize);
        calcScrollBars();
        update();
    }

    void TextEdit::remove(uint64_t pos, size_t size)
    {
        uint32_t row = 0, column = 0;
        calcRowColumn(pos, row, column);
        remove(row, column, size);
    }

    void TextEdit::remove(uint32_t row, uint32_t column, size_t size)
    {
        if (d->strings.empty()) return;
        if (row >= d->strings.size()) return;

        if (column > d->strings.at(row).size())
            column = d->strings.at(row).size();

        if (d->strings.at(row).size() - column >= size)
        {
            d->strings[row].erase(column, size);
            calcTextAreaSize(row);
        }
        else
        {
            const int eraseSize = d->strings.at(row).size() - column;
            d->strings[row].erase(column, eraseSize);

            uint32_t rowStart = row;
            uint32_t rowEnd = rowStart + 1;
            uint32_t columnEnd = size - 1;
            columnEnd -= eraseSize;

            for (uint32_t i=rowEnd; i<d->strings.size(); ++i)
            {
                if (columnEnd > d->strings.at(i).size())
                { ++rowEnd;  columnEnd -= d->strings.at(i).size(); }
                else break;
                --columnEnd;
            }

            if (rowEnd >= d->strings.size())
            {
                d->strings.erase(d->strings.begin() + rowStart + 1, d->strings.end());
                calcScrollBars();
            }
            else
            {
                d->strings[rowEnd].erase(0, columnEnd);

                if (rowEnd > rowStart)
                {
                    d->strings[rowStart].append(d->strings.at(rowEnd));

                    if (rowEnd + 1 <= d->strings.size()) ++rowEnd;
                    d->strings.erase(d->strings.begin() + rowStart + 1, d->strings.begin() + rowEnd);
                    calcScrollBars();
                }
            }
            calcTextAreaSize(rowStart, rowEnd - rowStart);
        }

        d->textLength -= size;
        update();
    }

    std::string TextEdit::text()
    {
        std::string result;
        result.reserve(d->textLength);

        for (int i=0; i<d->strings.size(); ++i)
        {
            result += d->strings[i];
            if (i < d->strings.size() - 1)
                result += "\n";
        }

        return std::move(result);
    }

    void TextEdit::clear()
    {
        d->strings.clear();
        d->rowsWidths.clear();
        d->textLength = 0;
        d->currentRow = 0;
        d->textShiftPos = {d->textIndent[SideLeft], d->textIndent[SideTop]};
        d->textCursorPos = {0, 0};
        d->currentColumn = 0;
        d->selection.selected = false;
        calcScrollBars();
        update();
    }

    size_t TextEdit::textSize()
    {
        return d->textLength;
    }

    size_t TextEdit::rowCount()
    {
        return d->strings.size();
    }

    size_t TextEdit::columnCount(uint32_t row)
    {
        if (row < d->strings.size())
            return d->strings.at(row).size();

        return 0;
    }

    bool TextEdit::isTextSelected()
    {
        return d->selection.selected;
    }

    std::string TextEdit::selectedText()
    {
        if (!d->selection.selected) return std::string();

        std::string result;

        if (d->selection.rowStart == d->selection.rowEnd)
        {
            const int posX = std::min(d->selection.columnStart, d->selection.columnEnd);
            const int posY = std::max(d->selection.columnStart, d->selection.columnEnd) - posX;
            result = d->strings.at(d->currentRow).substr(posX, posY);
        }
        else
        {
            uint32_t columnStart = d->selection.columnStart;
            uint32_t columnEnd = d->selection.columnEnd;
            uint32_t rowStart = d->selection.rowStart;
            uint32_t rowEnd = d->selection.rowEnd;

            if (d->selection.rowStart > d->selection.rowEnd){ std::swap(columnStart, columnEnd); std::swap(rowStart, rowEnd);}
            result.reserve((rowEnd - rowStart) * 50);
            result += d->strings.at(rowStart).substr(columnStart, d->strings.at(rowStart).size() - columnStart);
            result += "\n";

            const int rowMin = std::min(d->selection.rowStart, d->selection.rowEnd) + 1;
            const int rowMax = std::max(d->selection.rowStart, d->selection.rowEnd);
            for (int i=rowMin; i<rowMax; ++i) { result += d->strings.at(i);  result += "\n"; }

            result += d->strings.at(rowEnd).substr(0, columnEnd);
        }

        return result;
    }

    void TextEdit::removeSelectedText()
    {
        if(d->strings.empty()) return;
        if (!d->selection.selected) return;

        TextSelection selection = d->selection;
        if (d->selection.rowStart > d->selection.rowEnd)
        {
            std::swap(selection.posStart, selection.posEnd);
            std::swap(selection.rowStart, selection.rowEnd);
        }

        if (d->selection.columnStart > d->selection.columnEnd)
            std::swap(selection.columnStart, selection.columnEnd);

        TextAction action(TextAction::RemoveText, calcTextPos(selection.rowStart, selection.columnStart), selectedText());
        d->actions.push(action);

        remove(action.pos, action.string.size());
        d->selection.selected = false;
        d->currentRow = selection.rowStart;
        d->currentColumn = selection.columnStart;

        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;
        d->textCursorPos.x = textWidth(d->strings.at(d->currentRow).data(), d->currentColumn) + d->textShiftPos.x;

        update();
    }

    void TextEdit::selectAllText()
    {
        d->selection.selected = true;

        d->selection.columnStart = 0;
        d->selection.rowStart = 0;
        d->selection.posStart = 0;

        d->selection.rowEnd = d->strings.size() - 1;
        d->selection.columnEnd = d->strings.back().size();
        d->selection.posEnd = textWidth(d->strings.back());
        update();
    }

    void TextEdit::undo()
    {
        if (d->actions.empty()) return;

        TextAction action = std::move(d->actions.top());
        d->actions.pop();
        calcRowColumn(action.pos, d->currentRow, d->currentColumn);

        switch (action.type)
        {
        case TextAction::InsertChar: remove(action.pos, 1); break;
        case TextAction::InsertText: remove(action.pos, action.string.size()); break;
        case TextAction::RemoveChar: insert(action.pos, action.symbol); break;
        case TextAction::RemoveText: insert(action.pos, action.string); break;
        default: break;
        }
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

    void TextEdit::setTextCursorPos(uint64_t pos)
    {
        calcRowColumn(pos, d->currentRow, d->currentColumn);
        update();
    }

    void TextEdit::setTextCursorPos(uint32_t row, uint32_t column)
    {
        d->currentRow = row;
        d->currentColumn = column;

        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;
        d->textCursorPos.x = textWidth(d->strings.at(d->currentRow).data(), d->currentColumn) + d->textShiftPos.x;

        update();
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
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->backgrounds[static_cast<StyleState>(i)] = color;
        else d->backgrounds[state] = color;
    }

    void TextEdit::setSelectionColor(const Color &color)
    {
        d->selectionColor = color;
    }

    int TextEdit::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = 0;
        for (auto const& it : d->hoverHanders) if (it.first != ++id) break;
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
        d->mouseCursorPos = pos;

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
            d->mousePressPos = d->mouseCursorPos;

            calcCurrentRow();
            calcTextCursorPos();
            updateTextSelection(true);
            update();
        }
        else
        {
            if (d->mousePressPos == d->mouseCursorPos)
                d->selection.selected = false;
        }
    }

    void TextEdit::mouseWheelEvent(int32_t delta)
    {
        const int32_t verticalArea = height() - d->textIndent[SideTop] - d->textIndent[SideBottom];
        const int32_t textHeight = d->rowHeight * d->strings.size();

        if (textHeight < verticalArea)
        {
            if (d->textShiftPos.y < d->textIndent[SideTop])
            {
                d->textShiftPos.y = d->textIndent[SideTop];
                update();
            }
            return;
        }

        int32_t verticalShift = d->textShiftPos.y;
        if (delta > 0)
        {
            verticalShift += d->scrollRate;

            if (verticalShift > d->textIndent[SideTop])
                verticalShift = d->textIndent[SideTop];
        }
        else
        {
            verticalShift -= d->scrollRate;
            int32_t bottomShift = height() - textHeight - d->textIndent[SideBottom];

            if (verticalShift < bottomShift)
                verticalShift = bottomShift;
        }

        if (verticalShift == d->textShiftPos.y) return;

        d->textShiftPos.y = verticalShift;
        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;
        d->textCursorPos.x = textWidth(d->strings.at(d->currentRow).data(), d->currentColumn) + d->textShiftPos.x;
        d->scrollBarV->setValue(std::abs(d->textShiftPos.y));
        update();
    }

    void TextEdit::keyboardEvent(const KeyEvent &event)
    {
        if (!event.pressed) return;

        if (event.modifiers.ctrl)
        {
            switch (event.keycode)
            {
            case Key_Z: undo(); break;
            case Key_A: selectAllText(); break;
            case Key_C: Clipboard::instance().setText(selectedText()); break;
            case Key_X: Clipboard::instance().setText(selectedText()); removeSelectedText(); break;
            case Key_V: insertClipboardText(); break;
            default: break;
            }
            return;
        }

        char symbol = getCharacter(event);

        if (symbol != 0) keyReactionSymbol(symbol);
        else
        {
            switch (event.keycode)
            {
            case Key_Backspace: keyReactionBackspace(); break;
            case Key_Delete:    keyReactionDelete(); break;
            case Key_Return:    keyReactionReturn(); break;
            case Key_Left:      moveTextCursor(DirLeft); break;
            case Key_Right:     moveTextCursor(DirRight); break;
            case Key_Up:        moveTextCursor(DirUp); break;
            case Key_Down:      moveTextCursor(DirDown); break;
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

    void TextEdit::resizeEvent(const Size &size)
    {
        std::ignore = size;
        calcScrollBars();
    }

    void TextEdit::moveTextCursor(Direction dir)
    {
        switch (dir)
        {
        case DirLeft:
        {
            if (d->currentColumn > 0)
                --d->currentColumn;
            else
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
        case DirRight:
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
        case DirUp:
        {
            if (d->currentRow > 0)
            {
                --d->currentRow;
                if(d->textCursorPos.y < d->textIndent[SideTop]) d->textShiftPos.y += d->rowHeight;
            }
            else
            {
                const std::string &text = d->strings.at(d->currentRow);
                if (d->currentColumn > text.size()) d->currentColumn = text.size();
            }

            break;
        }
        case DirDown:
        {
            ++d->currentRow;

            if (d->currentRow >= d->strings.size())
            {
                d->currentRow = d->strings.size() - 1;
            }
            else
            {
                const std::string &text = d->strings.at(d->currentRow);
                if (d->currentColumn > text.size()) d->currentColumn = text.size();

                const int32_t visibleHeight = height() - d->textIndent[SideBottom] - d->textIndent[SideTop];
                if (d->textCursorPos.y + d->rowHeight > visibleHeight)
                    d->textShiftPos.y -= d->rowHeight;
            }

            break;
        }
        default: break;
        }

        d->textCursorPos.x = textWidth(d->strings.at(d->currentRow).data(), d->currentColumn) + d->textShiftPos.x;
        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;

        d->blinkState = true;
        d->selection.selected = false;

        update();
    }

    void TextEdit::updateTextSelection(bool justPressed)
    {
        if (justPressed)
        {
            d->selection.posStart = d->textCursorPos.x;
            d->selection.rowStart = d->currentRow;
            d->selection.columnStart = d->currentColumn;
        }

        d->selection.posEnd = d->textCursorPos.x;
        d->selection.rowEnd = d->currentRow;
        d->selection.columnEnd = d->currentColumn;

        d->selection.selected = (d->selection.posStart != d->selection.posEnd ||
                d->selection.rowStart != d->selection.rowEnd);
    }

    void TextEdit::insertClipboardText()
    {
        const std::string text = Clipboard::instance().getText();
        insert(d->currentRow, d->currentColumn, text);
        d->actions.push({TextAction::InsertText, calcTextPos(d->currentRow, d->currentColumn), text});
    }

    void TextEdit::onScrollVertical(uint32_t value)
    {
        d->textShiftPos.y = -value + d->textIndent[SideTop];
        update();
    }

    void TextEdit::onScrollHorizontal(uint32_t value)
    {
        d->textShiftPos.x = -value + d->textIndent[SideLeft];
        update();
    }

    void TextEdit::keyReactionSymbol(char symbol)
    {
        if (d->selection.selected) removeSelectedText();
        if (d->strings.empty()) d->strings.push_back(std::string());

        insert(d->currentRow, d->currentColumn, symbol);
        d->actions.push({TextAction::InsertChar, calcTextPos(d->currentRow, d->currentColumn), symbol});

        moveTextCursor(DirRight);
        calcScrollBars();
    }

    void TextEdit::keyReactionBackspace()
    {
        if (d->strings.empty()) return;

        if (d->selection.selected)
        {
            removeSelectedText();
        }
        else
        {
            if(d->currentColumn > 0)
            {
                moveTextCursor(DirLeft);

                d->actions.push({TextAction::RemoveChar, calcTextPos(d->currentRow, d->currentColumn),
                                 d->strings.at(d->currentRow).at(d->currentColumn)});

                remove(d->currentRow, d->currentColumn, 1);
            }
            else if(d->currentRow > 0)
            {
                if (d->currentRow < d->strings.size())
                {
                    const std::string &text = d->strings.at(d->currentRow);
                    moveTextCursor(DirLeft);
                    if (!text.empty()) d->strings[d->currentRow].append(text);
                    calcTextAreaSize(d->currentRow);
                }


                d->strings.erase(d->strings.begin() + d->currentRow + 1);
                d->rowsWidths.erase(d->rowsWidths.begin() + d->currentRow + 1);
                --d->textLength;

                d->actions.push({TextAction::RemoveChar, calcTextPos(d->currentRow, d->currentColumn), '\n'});
            }
            calcScrollBars();
        }
    }

    void TextEdit::keyReactionDelete()
    {
        if (d->strings.empty()) return;

        if (d->selection.selected)
        {
            removeSelectedText();
        }
        else if(d->currentRow < d->strings.size())
        {
            const std::string &text = d->strings.at(d->currentRow);
            if (text.empty())
            {
                d->strings.erase(d->strings.begin() + d->currentRow);
                d->rowsWidths.erase(d->rowsWidths.begin() + d->currentRow);
                --d->textLength;
                d->actions.push({TextAction::RemoveChar, calcTextPos(d->currentRow, d->currentColumn), '\n'});
                calcTextAreaSize(d->currentRow);
                calcScrollBars();
                return;
            }

            if ((text.size() - d->currentColumn) > 0)
            {
                d->actions.push({TextAction::RemoveChar, calcTextPos(d->currentRow, d->currentColumn),
                                 d->strings.at(d->currentRow).at(d->currentColumn)});

                d->strings[d->currentRow].erase(d->currentColumn, 1);
                --d->textLength;
                calcTextAreaSize(d->currentRow);
            }
            else if(d->currentRow + 1 < d->strings.size())
            {
                d->strings[d->currentRow].append(d->strings.at(d->currentRow + 1));
                d->strings.erase(d->strings.begin() + d->currentRow + 1);
                d->rowsWidths.erase(d->rowsWidths.begin() + d->currentRow + 1);
                --d->textLength;
                calcTextAreaSize(d->currentRow);

                d->actions.push({TextAction::RemoveChar, calcTextPos(d->currentRow, d->currentColumn), '\n'});
            }
            calcScrollBars();
        }
    }

    void TextEdit::keyReactionReturn()
    {
        if (d->selection.selected)
            removeSelectedText();

        size_t size = 0;
        if (d->strings.size() > d->currentRow)
            size = d->strings.at(d->currentRow).size();

        d->actions.push({TextAction::InsertChar, calcTextPos(d->currentRow, d->currentColumn), '\n'});

        if (d->currentColumn >= size)
        {
            ++d->currentRow;
            d->strings.insert(d->strings.begin() + d->currentRow, std::string());
            d->rowsWidths.insert(d->rowsWidths.begin() + d->currentRow, 0);
            calcTextAreaSize(d->currentRow);
        }
        else
        {
            const std::string &text = d->strings.at(d->currentRow);
            ++d->currentRow;
            d->strings.insert(d->strings.begin() + d->currentRow, text.substr(d->currentColumn));
            d->strings[d->currentRow - 1].erase(d->currentColumn, size - d->currentColumn);
            d->rowsWidths.insert(d->rowsWidths.begin() + d->currentRow, 0);
            calcTextAreaSize(d->currentRow - 1, 2);
        }

        d->currentColumn = 0;
        d->textCursorPos.x = d->textShiftPos.x;
        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;
        ++d->textLength;
        calcScrollBars();
    }

    void TextEdit::keyReactionHome()
    {
        d->currentColumn = 0;
        d->textCursorPos.x = d->textShiftPos.x;
        d->blinkState = true;
    }

    void TextEdit::keyReactionEnd()
    {
        d->currentColumn = d->strings.at(d->currentRow).size();
        d->textCursorPos.x = textWidth(d->strings.at(d->currentRow)) + d->textShiftPos.x;
        d->blinkState = true;
    }

    void TextEdit::keyReactionTab()
    {
        if (d->strings.empty()) return;
        d->actions.push({TextAction::InsertText, calcTextPos(d->currentRow, d->currentColumn), std::string("    ")});
        d->strings[d->currentRow].insert(d->currentColumn, 4, ' ');
        d->currentColumn += 3;
        d->textLength += 4;

        moveTextCursor(DirRight);
        calcTextAreaSize(d->currentRow);
    }

    void TextEdit::calcTextAreaSize(uint32_t row, uint32_t count)
    {
        uint32_t end = row + count;
        if (end > d->strings.size()) return;

        if (d->rowsWidths.size() != d->strings.size())
            d->rowsWidths.resize(d->strings.size());

        uint32_t maxWidth = 0;
        for (uint32_t i=0; i<d->strings.size(); ++i)
        {
            if (i >= row && i < end) d->rowsWidths[i] = textWidth(d->strings[i]);
            if (d->rowsWidths[i] > maxWidth) maxWidth = d->rowsWidths[i];
        }

        d->textAreaSize.width = maxWidth;
        d->textAreaSize.height = d->strings.size() * d->rowHeight;
    }

    void TextEdit::calcScrollBars()
    {
        uint32_t visibleHeight = height();
        visibleHeight -= d->textIndent[SideTop];
        visibleHeight -= d->textIndent[SideBottom];

        uint32_t visibleWidth = width();
        visibleWidth -= d->textIndent[SideLeft];
        visibleWidth -= d->textIndent[SideRight];

        uint16_t scrollBarWidth = 0;

        // Vertical
        if (d->textAreaSize.height > visibleHeight)
        {
            if (d->scrollBarV->isHidden()) d->scrollBarV->show();
            if (d->textAreaSize.width > visibleWidth) scrollBarWidth = d->scrollBarWidth;

            d->scrollBarV->setGeometry(width() - d->scrollBarWidth , 0,
                                       d->scrollBarWidth , height() - scrollBarWidth);

            d->scrollBarV->setRange(d->textAreaSize.height - visibleHeight);
        }
        else
        {
            d->scrollBarV->setRange(0);
            if (!d->scrollBarV->isHidden()) d->scrollBarV->hide();
        }

        // Horizontal
        if (d->textAreaSize.width > visibleWidth)
        {
            if (d->scrollBarH->isHidden()) d->scrollBarH->show();
            if (!d->scrollBarV->isHidden()) scrollBarWidth = d->scrollBarWidth;

            d->scrollBarH->setGeometry(0, height() - d->scrollBarWidth,
                                       width() - scrollBarWidth, d->scrollBarWidth);

            d->scrollBarH->setRange(d->textAreaSize.width - visibleWidth);
        }
        else
        {
            d->scrollBarH->setRange(0);
            if (!d->scrollBarH->isHidden()) d->scrollBarH->hide();
        }
    }

    void TextEdit::calcCurrentRow()
    {
        if (d->strings.empty()){ d->currentRow = 0; return; }

        d->currentRow = std::max(d->mouseCursorPos.y - d->textShiftPos.y, 0) / d->rowHeight;

        if (d->currentRow >= d->strings.size())
            d->currentRow = d->strings.size() - 1;

        d->textCursorPos.y = d->currentRow * d->rowHeight - 1 + d->textShiftPos.y;
    }

    void TextEdit::calcTextCursorPos()
    {
        if (d->strings.empty())
        {
            d->currentColumn = 0;
            d->textCursorPos.x = d->textShiftPos.x;
            return;
        }

        int delta = 3;

        if (d->mouseCursorPos.x <= delta + d->textShiftPos.x)
        {
            d->currentColumn = 0;
            d->textCursorPos.x = d->textShiftPos.x;
            return;
        }

        const std::string &text = d->strings.at(d->currentRow);
        size_t result = textWidth(text);
        int32_t shiftedX = d->mouseCursorPos.x - d->textShiftPos.x;

        if (shiftedX > result)
        {
            d->currentColumn = text.size();
            d->textCursorPos.x = result + d->textShiftPos.x;
            return;
        }

        int length = text.size();
        int half = text.size() / 2;

        for (size_t i=0; i<text.size(); ++i)
        {
            if (shiftedX > (result + delta)) length += half;
            else length -= half;

            if (half > 1) half = half / 2;

            result = textWidth(text.data(), length);
            if (std::abs(shiftedX - static_cast<int>(result)) <= delta) break;
        }

        d->currentColumn = length;
        d->textCursorPos.x = result + d->textShiftPos.x;
    }

    void TextEdit::calcRowColumn(uint64_t pos, uint32_t &row, uint32_t &column)
    {
        size_t steps = 0;
        row = 0; column = 0;
        for (const std::string &text: d->strings)
        {
            steps += text.size();
            if (steps >= pos) {  column = text.size() - (steps - pos); break; }
            ++steps;
            ++row;
        }
    }

    uint64_t TextEdit::calcTextPos(uint32_t row, uint32_t column)
    {
        uint32_t result = column;

        for (size_t i=0; i<d->strings.size(); ++i)
        {
            if (i < row) result += d->strings.at(i).size();
            else break;

            ++result;
        }

        return result;
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

    void TextEdit::drawBackground()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);
    }

    void TextEdit::drawTextSelection()
    {
        if (d->strings.empty()) return;
        if (!d->selection.selected) return;

        setBrush(d->selectionColor);
        setPen(d->selectionColor, 1);

        if (d->selection.rowStart == d->selection.rowEnd) // one line
        {
            const int posX = std::min(d->selection.posStart, d->selection.posEnd);
            const int posY = std::max(d->selection.posStart, d->selection.posEnd) - posX;
            drawRect(posX, d->selection.rowStart * d->rowHeight + d->textShiftPos.y, posY, d->rowHeight);
        }
        else
        {
            uint32_t posStart = d->selection.posStart;
            uint32_t posEnd = d->selection.posEnd;
            uint32_t rowStart = d->selection.rowStart;
            uint32_t rowEnd = d->selection.rowEnd;

            if (d->selection.rowStart > d->selection.rowEnd)
            { std::swap(posStart, posEnd); std::swap(rowStart, rowEnd);}

            // Top line of selection
            drawRect(posStart, rowStart * d->rowHeight + d->textShiftPos.y,
                     textWidth(d->strings.at(rowStart)) - posStart + d->textShiftPos.x, d->rowHeight);

            // Bottom line
            drawRect(d->textShiftPos.x, rowEnd * d->rowHeight + d->textShiftPos.y,
                     posEnd - d->textShiftPos.x, d->rowHeight);

            const int rowMin = std::min(d->selection.rowStart, d->selection.rowEnd) + 1;
            const int rowMax = std::max(d->selection.rowStart, d->selection.rowEnd);

            for (int i=rowMin; i<rowMax; ++i)
                drawRect(d->textShiftPos.x, i * d->rowHeight + d->textShiftPos.y,
                         textWidth(d->strings.at(i)), d->rowHeight);
        }
    }

    void TextEdit::drawTextStrings()
    {
        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 2);

        int row = -1;
        for (const std::string &text: d->strings)
            drawText(d->textShiftPos.x, ++row * d->rowHeight + d->textShiftPos.y, text);
    }

    void TextEdit::drawTextCursor()
    {
        if (!d->blinkState) return;

        setPen(d->textColors[d->styleState], 2);
        drawLine(d->textCursorPos.x, d->textCursorPos.y,
                 d->textCursorPos.x, d->textCursorPos.y + d->cursorHeight);
    }

} // namespace SA
