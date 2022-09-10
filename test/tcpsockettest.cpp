#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "tcpsockettest.h"

TcpSocketTest::TcpSocketTest(SA::Widget *parent) : SA::Widget(parent),
    m_textEditRead(this),
    m_textEditSend(this),
    m_lineEditPort(this),
    m_lineEditHost(this),
    m_labelHost("Server host:", this),
    m_labelPort("port:", this),
    m_labelTextSend("Data to send:", this),
    m_btnConnect("Connect", this),
    m_btnSend("Send", this)
{
    setTitle("TcpSocketTest");
    using namespace std::placeholders;
    m_btnConnect.addPressHandler(std::bind(&TcpSocketTest::btnConnectPressed, this, _1));
    m_btnSend.addPressHandler(std::bind(&TcpSocketTest::btnSendPressed, this, _1));
    m_tcpSocket.addReadHandler(std::bind(&TcpSocketTest::dataReaded, this, _1));
    m_tcpSocket.addDisconnectHandler([this](int){
        m_btnConnect.setText("Connect");
        m_textEditRead.append("=== Disconnected from server ===");
    });

    loadSettings();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

TcpSocketTest::~TcpSocketTest()
{
    saveSettings();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void TcpSocketTest::btnConnectPressed(bool state)
{
    if (!state) return;

    if (!m_tcpSocket.isConnected())
    {
        std::string host = m_lineEditHost.text();
        uint16_t port = 0;

        try {
            std::string strPort = m_lineEditPort.text();
            port = stoi(strPort);
        }  catch (...) {
            return;
        }


        if (m_tcpSocket.connect(host, port))
        {
            m_btnConnect.setText("Disconnect");
            m_textEditRead.append("=== Connected to server ===");
        }
    }
    else
    {
        m_tcpSocket.disconnect();
        if (!m_tcpSocket.isConnected())
        {
            m_btnConnect.setText("Connect");
            m_textEditRead.append("=== Disconnected from server ===");
        }
    }
}

void TcpSocketTest::btnSendPressed(bool state)
{
    if (!state) return;

    const std::string &strData = m_textEditSend.text();
    std::vector<char> data(strData.begin(), strData.end());

    m_tcpSocket.send(data);
}

void TcpSocketTest::dataReaded(const std::vector<char> &data)
{
    std::string strData(data.begin(), data.end());
    m_textEditRead.append(strData);
}

void TcpSocketTest::resizeEvent(const SA::Size &size)
{
    m_labelHost.setGeometry(5, 5, 110, 25);
    m_lineEditHost.setGeometry(m_labelHost.x() + m_labelHost.width() + 5, 5, width() - 400, 25);
    m_labelPort.setGeometry(m_lineEditHost.x() + m_lineEditHost.width() + 5, 5, 50, 25);
    m_lineEditPort.setGeometry(m_labelPort.x() + m_labelPort.width() + 5, 5, 100, 25);
    m_btnConnect.setGeometry(width() - 105, 5, 100, 25);
    m_textEditRead.setGeometry(5, 35, width() - 10, height() - 245);
    m_labelTextSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 8, 200, 25);
    m_textEditSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 35, width() - 10, 140);
    m_btnSend.setGeometry(width() - 100, height() - 30, 95, 25);
}

void TcpSocketTest::loadSettings()
{
    std::ifstream ifs("tcpsocket.conf");
    std::map<std::string, std::string> map;
    std::string line;

    if (ifs.is_open())
    {
        while (std::getline(ifs, line))
        {
            for (size_t i=0; i<line.size(); ++i)
            {
                if (line[i] == '=')
                    map[line.substr(0, i)] = line.substr(i + 1, line.size() - (i + 1));
            }
        }

        ifs.close();
    }

    m_lineEditPort.setText(map["port"]);
    m_lineEditHost.setText(map["host"]);
}

void TcpSocketTest::saveSettings()
{
    std::ofstream ofs("tcpsocket.conf");
    std::map<std::string, std::string> map;
    map["port"] = m_lineEditPort.text();
    map["host"] = m_lineEditHost.text();

    if (ofs.is_open())
    {
        for (auto it = map.begin(); it != map.end(); ++it)
            ofs << it->first << "=" << it->second << std::endl;

        ofs.close();
    }
}
