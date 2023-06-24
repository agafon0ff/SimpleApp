#pragma once

#include "widget.h"

class ImageTest : public SA::Widget
{
public:
    ImageTest(SA::Widget *parent = nullptr);
    ~ImageTest();

private:
    std::vector<uint8_t> pixmap;

    void loadPixmap();
    void paintEvent();
};

