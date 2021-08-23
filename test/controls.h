#pragma once

#include "widget.h"
#include "button.h"
#include "snakegame.h"

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

    SA::Button *m_button;
};
