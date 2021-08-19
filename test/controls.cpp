#include <iostream>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_button(new SA::Button(this))
{
    m_button->setGeometry(5, 5, 100, 40);
    m_button->show();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Controls::~Controls()
{
    delete m_button;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}
