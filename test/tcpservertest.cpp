#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>
#include "tcpservertest.h"

TcpServerTest::TcpServerTest(SA::Widget *parent) : SA::Widget(parent),
    m_textEditRead(this),
    m_textEditSend(this),
    m_lineEditPort(this),
    m_labelPort("Server port:", this),
    m_labelTextSend("Data to send:", this),
    m_btnStart("Start", this),
    m_btnSend("Send", this)
{
    setTitle("TcpServerTest");
    using namespace std::placeholders;
    m_btnStart.addPressHandler(std::bind(&TcpServerTest::btnStartPressed, this, _1));
    m_btnSend.addPressHandler(std::bind(&TcpServerTest::btnSendPressed, this, _1));
    m_tcpServer.addConnectHandler(std::bind(&TcpServerTest::onSocketConnected, this, _1, _2, _3));

    loadSettings();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

TcpServerTest::~TcpServerTest()
{
    m_sockets.clear();
    saveSettings();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void TcpServerTest::onSocketConnected(int sockDscr, uint32_t host, uint16_t port)
{
    m_disconnectedSocket.reset();

    auto socket = std::make_shared<SA::TcpSocket>();
    m_sockets.push_back(socket);

    socket->setDescriptor(sockDscr);
    socket->addReadHandler(std::bind(&TcpServerTest::dataReaded, this, std::placeholders::_1));
    socket->addDisconnectHandler(std::bind(&TcpServerTest::onSocketDisconnected, this, std::placeholders::_1));

    m_textEditRead.append("=== New socket connected: " + std::to_string(port) + " === " + std::to_string(m_sockets.size()));
}

void TcpServerTest::onSocketDisconnected(int sockDscr)
{
    auto it = m_sockets.end();

    for (it = m_sockets.begin(); it < m_sockets.end(); ++it) {
        SA::TcpSocket &socket = *(*it);
        if (socket.descriptor() == sockDscr) {
            m_disconnectedSocket = *it;
            break;
        }
    }


    if (it != m_sockets.end())
        m_sockets.erase(it);

    m_textEditRead.append("=== One socket disconnected === " + std::to_string(m_sockets.size()));
}

void TcpServerTest::btnStartPressed(bool state)
{
    if (!state) return;

    if (!m_tcpServer.isListen())
    {
        uint16_t port = 0;
        std::string strPort = m_lineEditPort.text();

        try {
            port = stoi(strPort);
        }  catch (...) {
            return;
        }

        if (m_tcpServer.listen(port))
        {
            m_btnStart.setText("Stop");
            m_textEditRead.append("=== Server started on port " + strPort + " ===");
        }
    }
    else
    {
        m_tcpServer.close();
        if (!m_tcpServer.isListen())
        {
            m_sockets.clear();
            m_btnStart.setText("Start");
            m_textEditRead.append("=== Server stopped === " + std::to_string(m_sockets.size()));
        }
    }
}

void TcpServerTest::btnSendPressed(bool state)
{
    if (!state) return;

    const std::string &strData = m_textEditSend.text();
    std::vector<char> data(strData.begin(), strData.end());

    for (auto &socket: m_sockets) {
        socket->send(data);
    }
}

void TcpServerTest::dataReaded(const std::vector<char> &data)
{
    std::string strData(data.begin(), data.end());
    m_textEditRead.append(strData);
}

void TcpServerTest::resizeEvent(const SA::Size &size)
{
    m_labelPort.setGeometry(5, 5, 120, 25);
    m_lineEditPort.setGeometry(m_labelPort.x() + m_labelPort.width() + 5, 5, 100, 25);
    m_btnStart.setGeometry(width() - 105, 5, 100, 25);
    m_textEditRead.setGeometry(5, 35, width() - 10, height() - 245);
    m_labelTextSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 8, 200, 25);
    m_textEditSend.setGeometry(5, m_textEditRead.y() + m_textEditRead.height() + 35, width() - 10, 140);
    m_btnSend.setGeometry(width() - 100, height() - 30, 95, 25);
}

void TcpServerTest::loadSettings()
{
    std::ifstream ifs("tcpserver.conf");
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
}

void TcpServerTest::saveSettings()
{
    std::ofstream ofs("tcpserver.conf");
    std::map<std::string, std::string> map;
    map["port"] = m_lineEditPort.text();

    if (ofs.is_open())
    {
        for (auto it = map.begin(); it != map.end(); ++it)
            ofs << it->first << "=" << it->second << std::endl;

        ofs.close();
    }
}
