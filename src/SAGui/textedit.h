#pragma once
#include <functional>
#include "SAGui/widget.h"

namespace SA
{
    class TextEdit : public Widget
    {
    public:

        enum StyleState
        {
            DisableState,
            EnableState,
            HoveredState,
            AllStates
        };

        explicit TextEdit(Widget *parent = nullptr);
        virtual ~TextEdit();

        void setText(const std::string &text);

        void append(char symbol);
        void append(const std::string &text);

        void insert(uint64_t pos, char symbol);
        void insert(uint32_t row, uint32_t column, char symbol);

        void insert(uint64_t pos, const std::string &text);
        void insert(uint32_t row, uint32_t column, const std::string &text);

        void remove(uint64_t pos, size_t size);
        void remove(uint32_t row, uint32_t column, size_t size);

        std::string text();
        void clear();

        size_t textSize();
        size_t rowCount();
        size_t columnCount(uint32_t row);

        bool isTextSelected();
        std::string selectedText();
        void removeSelectedText();
        void selectAllText();

        void undo();

        void setEnabled(bool state);
        bool isEnabled();

        void setTextColor(const Color &color, StyleState state = AllStates);
        void setBorder(const Pen &pen, StyleState state = AllStates);
        void setBackground(const Color &color, StyleState state = AllStates);
        void setSelectionColor(const Color &color);

        int addHoverHandler(const std::function<void (bool)> &func);
        void removeHoverHandler(int id);

    protected:
        virtual void timerEvent(int id);
        virtual void paintEvent();
        virtual void mouseHoverEvent(bool state);
        virtual void mouseMoveEvent(const Point &pos);
        virtual void mouseButtonEvent(const MouseEvent &event);
        virtual void keyboardEvent(const KeyEvent &event);
        virtual void focusEvent(bool state);

    private:
        void moveTextCursor(Direction dir);
        void updateTextSelection(bool justPressed = false);

        void keyReactionSymbol(char symbol);
        void keyReactionBackspace();
        void keyReactionDelete();
        void keyReactionReturn();
        void keyReactionHome();
        void keyReactionEnd();
        void keyReactionTab();

        void calcCurrentRow();
        void calcTextCursorPos();
        void calcRowColumn(uint64_t pos, uint32_t &row, uint32_t &column);
        uint64_t calcTextPos(uint32_t row, uint32_t column);
        void calcTextColors(const Color &color);
        void calcBorders(const Pen &pen);

        void drawBackground();
        void drawTextSelection();
        void drawTextStrings();
        void drawTextCursor();

        struct TextEditPrivate;
        TextEditPrivate * const d;

    }; // TextEdit

} // namespace SA
