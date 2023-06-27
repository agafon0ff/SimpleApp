#include <fstream>
#include <iterator>
#include <vector>
#include "imagetest.h"

ImageTest::ImageTest(SA::Widget *parent) : SA::Widget(parent)
{
    setTitle("RAV Image test");
    loadPixmap();
}

ImageTest::~ImageTest()
{

}

void ImageTest::loadPixmap()
{
    std::ifstream ifs("../../SimpleApp/test/cat200x200.raw", std::ios::binary);

    if (!ifs.is_open()) return;

    pixmap.reserve(ifs.tellg());

    ifs.unsetf(std::ios::skipws);
    ifs.seekg(0, std::ios::beg);

    std::copy(std::istream_iterator<uint8_t>(ifs),
              std::istream_iterator<uint8_t>(),
              std::back_inserter(pixmap));

    ifs.close();

    // convert RGBA -> BGRA
    for(int i = 0; i < pixmap.size(); i += 4)
    {
       uint8_t red  = pixmap[i+2];
       uint8_t blue = pixmap[i];
       pixmap[i]   = red;
       pixmap[i+2] = blue;
    }

    setIcon(pixmap, 200, 200);
}

void ImageTest::paintEvent()
{
    if (pixmap.empty()) return;
    drawImage(pixmap, {50, 50, 200, 200});
}
