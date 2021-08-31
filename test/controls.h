#pragma once

#include "widget.h"
#include "button.h"
#include "textedit.h"
#include "snakegame.h"

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

    SA::Button *m_button1 = nullptr;
    SA::Button *m_button2 = nullptr;
    SA::Button *m_button3 = nullptr;
    SA::Button *m_button4 = nullptr;
    SA::TextEdit *m_textEdit = nullptr;
    SA::TextEdit *m_textEdit2 = nullptr;

    void btnPressed(bool state);
    void btnHovered(bool state);
    void btnChecked(bool state);
};


