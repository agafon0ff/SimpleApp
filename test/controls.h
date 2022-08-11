#pragma once

#include "structs.h"
#include "widget.h"
#include "button.h"
#include "textedit.h"
#include "lineedit.h"
#include "label.h"

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

private:
    SA::TextEdit m_textEdit;
    SA::LineEdit m_lineEdit;
    SA::Button m_buttonAdd;
    SA::Label m_label;

    void btnAddPressed(bool state);
    void resizeEvent(const SA::Size &size);
};


