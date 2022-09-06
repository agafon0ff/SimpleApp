#pragma once

#include <vector>
#include "structs.h"
#include "widget.h"
#include "button.h"
#include "textedit.h"
#include "lineedit.h"
#include "label.h"
#include "tcpserver.h"
#include "tcpsocket.h"

class TcpServerTest : public SA::Widget
{
public:
    TcpServerTest(SA::Widget *parent = nullptr);
    ~TcpServerTest();

private:
    SA::TextEdit m_textEditRead;
    SA::TextEdit m_textEditSend;
    SA::LineEdit m_lineEditPort;
    SA::Label m_labelPort;
    SA::Label m_labelTextSend;
    SA::Button m_btnStart;
    SA::Button m_btnSend;
    SA::TcpServer m_tcpServer;

    std::vector<std::shared_ptr<SA::TcpSocket> > m_sockets;
    std::shared_ptr<SA::TcpSocket> m_disconnectedSocket;

    void onSocketConnected(int sockDscr, uint32_t host, uint16_t port);
    void onSocketDisconnected(int sockDscr);
    void btnStartPressed(bool state);
    void btnSendPressed(bool state);
    void dataReaded(const std::vector<char> &data);
    void resizeEvent(const SA::Size &size);
    void loadSettings();
    void saveSettings();
};

