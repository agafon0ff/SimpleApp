#pragma once
#include <functional>
#include "widget.h"
#include "global.h"

namespace SA
{
    class LineEdit : public Widget
    {
    public:
        explicit LineEdit(Widget *parent = nullptr);
        ~LineEdit();

        void setText(const std::string &text);

        void append(char symbol);
        void append(const std::string &text);

        void insert(uint64_t pos, char symbol);
        void insert(uint64_t pos, const std::string &text);

        void remove(uint64_t pos, size_t size);

        std::string text();
        void clear();

        size_t textSize();

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

        int addReturnHandler(const std::function<void ()> &func);
        void removeReturnHandler(int id);

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
        void insertClipboardText();

        void keyReactionSymbol(char symbol);
        void keyReactionBackspace();
        void keyReactionDelete();
        void keyReactionReturn();
        void keyReactionHome();
        void keyReactionEnd();

        void calcTextCursorPos();
        void calcTextColors(const Color &color);
        void calcBorders(const Pen &pen);

        void drawBackground();
        void drawSelection();
        void drawTextString();
        void drawCursor();

        struct LineEditPrivate;
        LineEditPrivate * const d;

    }; // class LineEdit
} // namespace SA
