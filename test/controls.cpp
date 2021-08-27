#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_button(new SA::Button(this))
{
    using namespace std::placeholders;
    m_button->setGeometry(5, 5, 120, 30);
    m_button->addPressHandler(std::bind(&Controls::btnPressed, this, _1));
    m_button->addHoverHandler(std::bind(&Controls::btnHovered, this, _1));

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Controls::~Controls()
{
    delete m_button;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Controls::btnPressed(bool state)
{
    std::cout << __PRETTY_FUNCTION__
              << " state: " << state
              << std::endl;
}

void Controls::btnHovered(bool state)
{
    std::cout << __PRETTY_FUNCTION__
              << " state: " << state
              << std::endl;
}
