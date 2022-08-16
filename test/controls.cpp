#include <iostream>
#include <functional>
#include "controls.h"

Controls::Controls(SA::Widget *parent) : SA::Widget(parent),
    m_textEdit(this),
    m_checkBox("Clear text after adding", this),
    m_lineEdit(this),
    m_buttonAdd("Add", this),
    m_label("Text:", this)
{
    setTitle("Controls");
    using namespace std::placeholders;

    m_buttonAdd.addPressHandler(std::bind(&Controls::btnAddPressed, this, _1));
    m_textEdit.setText("    Lorem Ipsum is simply dummy\n"
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
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Controls::btnAddPressed(bool state)
{
    if (!state) return;
    m_textEdit.append(m_lineEdit.text());

    if (m_checkBox.isChecked())
        m_lineEdit.clear();
}

void Controls::resizeEvent(const SA::Size &size)
{
    m_textEdit.setGeometry(5, 5, size.width - 10, size.height - 65);
    m_checkBox.setGeometry(5, size.height - 57, size.width - 10, 25);
    m_label.setGeometry(5, size.height - 30, 55, 25);
    m_lineEdit.setGeometry(60, size.height - 30, size.width - 170, 25);
    m_buttonAdd.setGeometry(60 + m_lineEdit.width() + 5, size.height - 30, 100, 25);
}
