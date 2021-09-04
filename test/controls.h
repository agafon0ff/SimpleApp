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

private:
    SA::Button *m_button1 = nullptr;
    SA::TextEdit *m_textEdit = nullptr;

    void btnPressed(bool state);
    void resizeEvent(int width, int height);
};


