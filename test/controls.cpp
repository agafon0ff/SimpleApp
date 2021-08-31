#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_button1(new SA::Button("Button 1", this)),
    m_button2(new SA::Button("Button 2", this)),
    m_button3(new SA::Button("Button 3", this)),
    m_button4(new SA::Button("Button 4", this)),
    m_textEdit(new SA::TextEdit(this))
{
    using namespace std::placeholders;
    m_button1->setGeometry(5, 5, 120, 30);
    m_button2->setGeometry(130, 5, 120, 30);
    m_button3->setGeometry(5, 40, 120, 30);
    m_button4->setGeometry(130, 40, 120, 30);

    m_textEdit->setGeometry(5, 80, 500, 500);
    m_textEdit->setText("Hello World!");

    m_button1->addCheckHandler(std::bind(&Controls::btnChecked, this, _1));
    m_button2->addPressHandler(std::bind(&Controls::btnPressed, this, _1));
    m_button3->addHoverHandler(std::bind(&Controls::btnHovered, this, _1));

    m_button1->setCheckable(true);
    m_button4->setEnabled(false);

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

void Controls::btnChecked(bool state)
{
    std::cout << __PRETTY_FUNCTION__
              << " state: " << state
              << std::endl;
}
