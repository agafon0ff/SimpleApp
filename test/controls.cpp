#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_textEdit(new SA::TextEdit(this)),
    m_lineEdit(new SA::LineEdit(this)),
    m_buttonAdd(new SA::Button("Add", this))
{
    using namespace std::placeholders;

    m_buttonAdd->addPressHandler(std::bind(&Controls::btnAddPressed, this, _1));
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
                        "versions of Lorem Ipsum.\n");

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Controls::~Controls()
{
    delete m_buttonAdd;
    delete m_lineEdit;
    delete m_textEdit;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Controls::btnAddPressed(bool state)
{
    if (!state) return;
    m_textEdit->append(m_lineEdit->text());
    m_lineEdit->clear();
}

void Controls::resizeEvent(const SA::Size &size)
{
    m_textEdit->setGeometry(5, 5, size.width - 10, size.height - 40);
    m_buttonAdd->setGeometry(size.width - 105, size.height - 30, 100, 25);
    m_lineEdit->setGeometry(5, size.height - 30, size.width - 115, 25);
}
