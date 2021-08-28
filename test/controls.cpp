#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_button1(new SA::Button(this)),
    m_button2(new SA::Button(this)),
    m_button3(new SA::Button(this)),
    m_button4(new SA::Button(this))
{
    using namespace std::placeholders;
    m_button1->setGeometry(5, 5, 120, 30);
    m_button2->setGeometry(125, 5, 120, 30);
    m_button3->setGeometry(5, 35, 120, 30);
    m_button4->setGeometry(125, 35, 120, 30);
    m_button1->addPressHandler(std::bind(&Controls::btnPressed, this, _1));
    m_button2->addPressHandler(std::bind(&Controls::btnPressed, this, _1));
    m_button3->addPressHandler(std::bind(&Controls::btnPressed, this, _1));
    m_button4->addPressHandler(std::bind(&Controls::btnPressed, this, _1));

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Controls::~Controls()
{
    delete m_button1;
    delete m_button2;
    delete m_button3;
    delete m_button4;
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
