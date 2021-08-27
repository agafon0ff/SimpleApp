#pragma once

#include "widget.h"
#include "button.h"
#include "snakegame.h"

#include <iostream>

class Controls : public SA::Widget
{
public:
    Controls(SA::Widget *parent = nullptr);
    ~Controls();

    SA::Button *m_button = nullptr;

    void btnPressed(bool state);
    void btnHovered(bool state);
};


