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
        void append(const std::string &text);
        void insert(const std::string &text, uint32_t row, uint32_t column);
        std::string text();
        void clear();
        size_t textSize();
        size_t rowCount();

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
        void calcTextColors(const Color &color);
        void calcBorders(const Pen &pen);
        void calcBackgrounds(const Color &color);

        void drawBackground();
        void drawTextSelection();
        void drawTextStrings();
        void drawTextCursor();

        struct TextEditPrivate;
        TextEditPrivate * const d;

    }; // TextEdit

} // namespace SA
