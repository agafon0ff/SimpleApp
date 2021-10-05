#pragma once

#include "SACore/structs.h"
#include "SAGui/widget.h"
#include "SAGui/button.h"
#include "SAGui/textedit.h"
#include "SAGui/lineedit.h"
#include "snakegame.h"

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

private:
    SA::TextEdit *m_textEdit = nullptr;
    SA::LineEdit *m_lineEdit = nullptr;
    SA::Button *m_buttonAdd = nullptr;

    void btnAddPressed(bool state);
    void resizeEvent(const SA::Size &size);
};


