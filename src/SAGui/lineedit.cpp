#include <iostream>
#include <string>
#include <stack>

#include "lineedit.h"
#include "clipboard.h"
#include "utility.h"

using std::cout;
using std::endl;

namespace SA
{
    struct LineEdit::LineEditPrivate
    {
        std::string text;
        std::stack<TextAction> actions;

        int timerId = 0;

        Point mouseCursorPos;
        Point mousePressPos;
        Point textShiftPos = {5, 5};

        uint32_t textCursorX = 0;
        uint32_t cursorHeight = 10;
        uint32_t currentColumn = 0;
        int32_t  textIndent[sizeof(Side)] = {5, 5, 5, 5};

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
        Color selectionColor = {140, 140, 140};

        // Events listeners
        std::map<int, std::function<void (bool)> > hoverHandlers;
        std::map<int, std::function<void ()> > returnHandlers;
    };

    LineEdit::LineEdit(Widget *parent) : Widget(parent),
        d(new LineEditPrivate)
    {
        d->timerId = startTimer(500);
        d->cursorHeight = textHeight() + 1;

        resize(150, 26);
        setCursorShape(Text);
        calcTextColors({240, 240, 240});
        calcBorders({90, 90, 90, 1});
        setBackground({30, 30, 30});
    }

    LineEdit::~LineEdit()
    {
        delete d;
    }

    void LineEdit::setText(const std::string &text)
    {
        clear();
        append(text);
    }

    void LineEdit::append(char symbol)
    {
        d->text += symbol;
        update();
    }

    void LineEdit::append(const std::string &text)
    {
        d->text.append(text);
        update();
    }

    void LineEdit::insert(uint64_t pos, char symbol)
    {
        if (pos > d->text.size()) return;
        d->text.insert(pos, 1, symbol);
        update();
    }

    void LineEdit::insert(uint64_t pos, const std::string &text)
    {
        if (pos > d->text.size()) return;
        d->text.insert(pos, text);
        update();
    }

    void LineEdit::remove(uint64_t pos, size_t size)
    {
        if (pos + size > d->text.size()) return;
        d->text.erase(pos, size);
    }

    std::string LineEdit::text()
    {
        return d->text;
    }

    void LineEdit::clear()
    {
        d->text.clear();
        d->textCursorX = 0;
        d->selection.selected = false;
        d->textShiftPos = {d->textIndent[SideLeft], d->textIndent[SideTop]};
        update();
    }

    size_t LineEdit::textSize()
    {
        return d->text.size();
    }

    bool LineEdit::isTextSelected()
    {
        return d->selection.selected;
    }

    std::string LineEdit::selectedText()
    {
        if (!d->selection.selected) return std::string();

        const int posX = std::min(d->selection.columnStart, d->selection.columnEnd);
        const int posY = std::max(d->selection.columnStart, d->selection.columnEnd) - posX;
        return d->text.substr(posX, posY);
    }

    void LineEdit::removeSelectedText()
    {
        if(d->text.empty()) return;
        if (!d->selection.selected) return;

        TextSelection selection = d->selection;

        if (d->selection.posStart > d->selection.posEnd)
        {
            std::swap(selection.columnStart, selection.columnEnd);
            std::swap(selection.posStart, selection.posEnd);
        }

        TextAction action(TextAction::RemoveText, selection.columnStart, selectedText());
        d->actions.push(action);
        d->selection.selected = false;
        d->currentColumn = selection.columnStart;
        remove(action.pos, action.string.size());

        d->textCursorX = textWidth(d->text.data(), d->currentColumn) + d->textShiftPos.x;
        update();
    }

    void LineEdit::selectAllText()
    {
        d->selection.selected = true;
        d->selection.columnStart = 0;
        d->selection.posStart = 0;
        d->selection.columnEnd = d->text.size();
        d->selection.posEnd = textWidth(d->text);
        update();
    }

    void LineEdit::undo()
    {
        if (d->actions.empty()) return;

        TextAction action = std::move(d->actions.top());
        d->actions.pop();

        switch (action.type)
        {
        case TextAction::InsertChar: remove(action.pos, 1); break;
        case TextAction::InsertText: remove(action.pos, action.string.size()); break;
        case TextAction::RemoveChar: insert(action.pos, action.symbol); break;
        case TextAction::RemoveText: insert(action.pos, action.string); break;
        default: break;
        }
    }

    void LineEdit::setEnabled(bool state)
    {
        d->enable = state;
        d->styleState = state ? EnableState : DisableState;
        update();
    }

    bool LineEdit::isEnabled()
    {
        return d->enable;
    }

    void LineEdit::setTextColor(const Color &color, StyleState state)
    {
        if (state == AllStates) calcTextColors(color);
        else d->textColors[state] = color;
    }

    void LineEdit::setBorder(const Pen &pen, StyleState state)
    {
        if (state == AllStates) calcBorders(pen);
        else d->borderPens[state] = pen;
    }

    void LineEdit::setBackground(const Color &color, StyleState state)
    {
        if (state == AllStates)
            for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
                d->backgrounds[static_cast<StyleState>(i)] = color;
        else d->backgrounds[state] = color;
    }

    void LineEdit::setSelectionColor(const Color &color)
    {
        d->selectionColor = color;
    }

    int LineEdit::addHoverHandler(const std::function<void (bool)> &func)
    {
        int id = static_cast<int>(d->hoverHandlers.size());
        for (auto const& it : d->hoverHandlers) if (it.first != ++id) break;
        d->hoverHandlers.insert({id, func});
        return id;
    }

    void LineEdit::removeHoverHandler(int id)
    {
        auto it = d->hoverHandlers.find(id);
        if (it != d->hoverHandlers.end())
            d->hoverHandlers.erase(it);
    }

    int LineEdit::addReturnHandler(const std::function<void ()> &func)
    {
        int id = static_cast<int>(d->returnHandlers.size());
        for (auto const& it : d->returnHandlers) if (it.first != ++id) break;
        d->returnHandlers.insert({id, func});
        return id;
    }

    void LineEdit::removeReturnHandler(int id)
    {
        auto it = d->returnHandlers.find(id);
        if (it != d->returnHandlers.end())
            d->returnHandlers.erase(it);
    }

    void LineEdit::timerEvent(int id)
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

    void LineEdit::paintEvent()
    {
        drawBackground();
        drawSelection();
        drawTextString();
        drawCursor();
    }

    void LineEdit::mouseHoverEvent(bool state)
    {
        if (!d->enable) return;

        d->styleState = state ? HoveredState : EnableState;
        update();
        for (const auto &it: d->hoverHandlers) it.second(state);
    }

    void LineEdit::mouseMoveEvent(const Point &pos)
    {
        d->mouseCursorPos = pos;

        if (d->pressed)
        {
            calcTextCursorPos();
            updateTextSelection();
            update();
        }
    }

    void LineEdit::mouseButtonEvent(const MouseEvent &event)
    {
        if (!d->enable) return;
        if (event.button != ButtonLeft) return;

        d->pressed = event.pressed;

        if (event.pressed)
        {
            d->mousePressPos = d->mouseCursorPos;

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

    void LineEdit::keyboardEvent(const KeyEvent &event)
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
            default: break;
            }
        }

        update();
    }

    void LineEdit::focusEvent(bool state)
    {
        d->inFocus = state;
    }

    void LineEdit::moveTextCursor(Direction dir)
    {
        switch (dir)
        {
        case DirLeft:
        {
            if (d->currentColumn > 0)
                --d->currentColumn;

            break;
        }
        case DirRight:
        {
            ++d->currentColumn;

            if (d->currentColumn > d->text.size())
                d->currentColumn = d->text.size();

            break;
        }
        default: break;
        }

        d->textCursorX = textWidth(d->text.data(), d->currentColumn) + d->textShiftPos.x;

        d->blinkState = true;
        d->selection.selected = false;

        update();
    }

    void LineEdit::updateTextSelection(bool justPressed)
    {
        if (justPressed)
        {
            d->selection.posStart = d->textCursorX;
            d->selection.columnStart = d->currentColumn;
        }

        d->selection.posEnd = d->textCursorX;
        d->selection.columnEnd = d->currentColumn;

        d->selection.selected = d->selection.posStart != d->selection.posEnd;
    }

    void LineEdit::insertClipboardText()
    {
        const std::string text = Clipboard::instance().getText();
        insert(d->currentColumn, text);
        d->actions.push({TextAction::InsertText, d->currentColumn, text});
    }

    void LineEdit::keyReactionSymbol(char symbol)
    {
        if (d->selection.selected) removeSelectedText();

        insert(d->currentColumn, symbol);
        d->actions.push({TextAction::InsertChar, d->currentColumn, symbol});

        moveTextCursor(DirRight);
    }

    void LineEdit::keyReactionBackspace()
    {
        if (d->text.empty()) return;

        if (d->selection.selected)
        {
            removeSelectedText();
        }
        else
        {
            if(d->currentColumn > 0)
            {
                moveTextCursor(DirLeft);
                d->actions.push({TextAction::RemoveChar, d->currentColumn, d->text.at(d->currentColumn)});
                remove(d->currentColumn, 1);
            }
        }
    }

    void LineEdit::keyReactionDelete()
    {
        if (d->text.empty()) return;

        if (d->selection.selected)
        {
            removeSelectedText();
        }
        else
        {
            if ((d->text.size() - d->currentColumn) > 0)
            {
                d->actions.push({TextAction::RemoveChar, d->currentColumn, d->text.at(d->currentColumn)});
                remove(d->currentColumn, 1);
            }
        }
    }

    void LineEdit::keyReactionReturn()
    {
        for (const auto &it: d->returnHandlers) it.second();
    }

    void LineEdit::keyReactionHome()
    {
        d->currentColumn = 0;
        d->textCursorX = d->textShiftPos.x;
        d->blinkState = true;
    }

    void LineEdit::keyReactionEnd()
    {
        d->currentColumn = d->text.size();
        d->textCursorX = textWidth(d->text) + d->textShiftPos.x;
        d->blinkState = true;
    }

    void LineEdit::calcTextCursorPos()
    {
        if (d->text.empty())
        {
            d->currentColumn = 0;
            d->textCursorX = d->textShiftPos.x;
            return;
        }

        int delta = 3;

        if (d->mouseCursorPos.x <= delta + d->textShiftPos.x)
        {
            d->currentColumn = 0;
            d->textCursorX = d->textShiftPos.x;
            return;
        }

        size_t result = textWidth(d->text);
        int32_t shiftedX = d->mouseCursorPos.x - d->textShiftPos.x;

        if (shiftedX > result)
        {
            d->currentColumn = d->text.size();
            d->textCursorX = result + d->textShiftPos.x;
            return;
        }

        int length = d->text.size();
        int half = d->text.size() / 2;

        for (size_t i=0; i<d->text.size(); ++i)
        {
            if (shiftedX > (result + delta)) length += half;
            else length -= half;

            if (half > 1) half = half / 2;

            result = textWidth(d->text.data(), length);
            if (std::abs(shiftedX - static_cast<int>(result)) <= delta) break;
        }

        d->currentColumn = length;
        d->textCursorX = result + d->textShiftPos.x;
    }

    void LineEdit::calcTextColors(const Color &color)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(color, d->textColors[static_cast<StyleState>(i)], 6 * i);
    }

    void LineEdit::calcBorders(const Pen &pen)
    {
        for (int i=static_cast<int>(DisableState); i<static_cast<int>(AllStates); ++i)
            darker(pen.color, d->borderPens[static_cast<StyleState>(i)].color, 6 * i);
    }

    void LineEdit::drawBackground()
    {
        setBrush(d->backgrounds[d->styleState]);
        setPen(d->borderPens[d->styleState]);
        drawRect(0, 0, width() - 1, height() - 1);
    }

    void LineEdit::drawSelection()
    {
        if (d->text.empty()) return;
        if (!d->selection.selected) return;

        setBrush(d->selectionColor);
        setPen(d->selectionColor, 1);

        const int posX = std::min(d->selection.posStart, d->selection.posEnd);
        const int posY = std::max(d->selection.posStart, d->selection.posEnd) - posX;
        drawRect(posX, d->selection.rowStart * d->cursorHeight + d->textShiftPos.y, posY, d->cursorHeight);
    }

    void LineEdit::drawTextString()
    {
        setBrush(d->textColors[d->styleState]);
        setPen(d->textColors[d->styleState], 2);
        drawText(d->textShiftPos.x, d->textShiftPos.y, d->text);
    }

    void LineEdit::drawCursor()
    {
        if (!d->blinkState) return;

        setPen(d->textColors[d->styleState], 2);
        drawLine(d->textCursorX, d->textShiftPos.y - 2,
                 d->textCursorX, d->textShiftPos.y + d->cursorHeight);
    }

} // namespace SA
