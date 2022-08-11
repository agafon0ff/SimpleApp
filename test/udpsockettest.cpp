#include <iostream>
#include <functional>
#include "udpsockettest.h"

UdpSocketTest::UdpSocketTest(SA::Widget *parent) : SA::Widget(parent),
    m_textEditRead(this),
    m_textEditSend(this),
    m_lineEditPortRead(this),
    m_lineEditPortSend(this),
    m_lineEditHostSend(this),
    m_labelPortRead("Port bind:", this),
    m_labelHostSend("Send to host:", this),
    m_labelPortSend("port:", this),
    m_labelTextSend("Data to send:", this),
    m_labelTextRead("Readed data:", this),
    m_btnBind("Start bind", this),
    m_btnSend("Send", this)
{
    using namespace std::placeholders;
    m_btnBind.addPressHandler(std::bind(&UdpSocketTest::btnBindPressed, this, _1));
    m_btnSend.addPressHandler(std::bind(&UdpSocketTest::btnSendPressed, this, _1));
    m_udpSocket.addReadHandler(std::bind(&UdpSocketTest::dataReaded, this, _1));

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

UdpSocketTest::~UdpSocketTest()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void UdpSocketTest::btnBindPressed(bool state)
{
    if (!state) return;

    if (!m_udpSocket.isBinded())
    {
        uint16_t port = 0;

        try {
            std::string strPort = m_lineEditPortRead.text();
            port = stoi(strPort);
        }  catch (...) {
            return;
        }

        if (m_udpSocket.bind(port))
            m_btnBind.setText("Stop bind");
    }
    else
    {
        m_udpSocket.unbind();
        if (!m_udpSocket.isBinded())
            m_btnBind.setText("Start bind");
    }
}

void UdpSocketTest::btnSendPressed(bool state)
{
    if (!state) return;

    uint16_t port = 0;

    try {
        std::string strPort = m_lineEditPortSend.text();
        port = stoi(strPort);
    }  catch (...) {
        return;
    }

    std::string host = m_lineEditHostSend.text();
    const std::string &strData = m_textEditSend.text();
    std::vector<char> data(strData.begin(), strData.end());

    m_udpSocket.send(data, host, port);
}

void UdpSocketTest::dataReaded(const std::vector<char> &data)
{
    std::string strData(data.begin(), data.end());
    m_textEditRead.append(strData);
}

void UdpSocketTest::resizeEvent(const SA::Size &size)
{
    m_labelPortRead.setGeometry(width() - 335, 5, 100, 25);
    m_lineEditPortRead.setGeometry(width() - 235, 5, 100, 25);
    m_btnBind.setGeometry(width() - 130, 5, 125, 25);

    m_labelTextRead.setGeometry(5, 8, 120, 25);
    m_textEditRead.setGeometry(5, 35, width() - 10, height() - 245);

    m_labelTextSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 8, 200, 25);
    m_textEditSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 35, width() - 10, 140);

    m_labelHostSend.setGeometry(5, height() - 30, 120, 25);
    m_lineEditHostSend.setGeometry(m_labelHostSend.x() + m_labelHostSend.width() + 5,
                                   height() - 30, width() - 400, 25);

    m_labelPortSend.setGeometry(m_lineEditHostSend.x() + m_lineEditHostSend.width() + 5,
                                height() - 30, 50, 25);

    m_lineEditPortSend.setGeometry(m_labelPortSend.x() + m_labelPortSend.width() + 5,
                                   height() - 30, 100, 25);

    m_btnSend.setGeometry(width() - 100, height() - 30, 95, 25);
}
