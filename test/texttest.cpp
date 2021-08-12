#include <iostream>
#include "texttest.h"

TextTest::TextTest(SA::Widget *parent) : SA::Widget(parent)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

TextTest::~TextTest()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void TextTest::paintEvent()
{
    setPen(1, 30, 30, 30);
    setBrush(220, 220, 220);
    drawRect(20, 20, 100, 30);

    setPen(1, 30, 30, 30);
    drawText(26, 26, "Simpe Text");
}
