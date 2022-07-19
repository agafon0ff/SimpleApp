#pragma once

#include "SACore/structs.h"
#include "SAGui/widget.h"
#include "SAGui/button.h"
#include "SAGui/textedit.h"
#include "SAGui/lineedit.h"
#include "SAGui/label.h"

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


