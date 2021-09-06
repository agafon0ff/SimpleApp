#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_button1(new SA::Button("Button", this)),
    m_textEdit(new SA::TextEdit(this))
{
    using namespace std::placeholders;

    m_button1->addPressHandler(std::bind(&Controls::btnPressed, this, _1));

    m_textEdit->setText("    Lorem Ipsum is simply dummy\n"
                        "text of the printing and typesetting\n"
                        "industry. Lorem Ipsum has been the\n"
                        "industry's standard dummy text ever\n"
                        "since the 1500s, when an unknown printer\n"
                        "took a galley of type and scrambled\n"
                        "it to make a type specimen book.\n\n"
                        "    It has survived not only five centuries,\n"
                        "but also the leap into electronic\n"
                        "typesetting, remaining essentially\n"
                        "unchanged. It was popularised in\n"
                        "the 1960s with the release of Letraset\n"
                        "sheets containing Lorem Ipsum passages,\n"
                        "and more recently with desktop publishing\n"
                        "software like Aldus PageMaker including\n"
                        "versions of Lorem Ipsum.");

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Controls::~Controls()
{
    delete m_button1;
    delete m_textEdit;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Controls::btnPressed(bool state)
{
    std::cout << __PRETTY_FUNCTION__
              << " state: " << state
              << std::endl;
}

void Controls::resizeEvent(const SA::Size &size)
{
    m_textEdit->setGeometry(5, 5, size.width - 10, size.height - 40);
    m_button1->setGeometry(size.width - 105, size.height - 30, 100, 25);
}
