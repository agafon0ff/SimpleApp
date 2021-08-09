#pragma once

#include "SAGui/widget.h"

class TextTest : public SA::Widget
{
public:
    TextTest(SA::Widget *parent = nullptr);
    ~TextTest();

    void paintEvent();
};
