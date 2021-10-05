#pragma once

#include "SACore/structs.h"
#include "SAGui/widget.h"
#include "SAGui/button.h"
#include "SAGui/textedit.h"
#include "snakegame.h"

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

private:
    SA::Button *m_buttonClear = nullptr;
    SA::Button *m_buttonAdd = nullptr;
    SA::TextEdit *m_textEdit = nullptr;

    void btnClearPressed(bool state);
    void btnAddPressed(bool state);
    void resizeEvent(const SA::Size &size);
};


