#pragma once

#include "structs.h"
#include "widget.h"
#include "button.h"
#include "textedit.h"
#include "lineedit.h"
#include "label.h"
#include "tcpsocket.h"

class TcpSocketTest : public SA::Widget
{
public:
    TcpSocketTest(SA::Widget *parent = nullptr);
    ~TcpSocketTest();

private:
    SA::TextEdit m_textEditRead;
    SA::TextEdit m_textEditSend;
    SA::LineEdit m_lineEditPort;
    SA::LineEdit m_lineEditHost;
    SA::Label m_labelPort;
    SA::Label m_labelHost;
    SA::Label m_labelTextSend;
    SA::Button m_btnConnect;
    SA::Button m_btnSend;
    SA::TcpSocket m_tcpSocket;

    void btnConnectPressed(bool state);
    void btnSendPressed(bool state);
    void dataReaded(const std::vector<char> &data);
    void resizeEvent(const SA::Size &size);
    void loadSettings();
    void saveSettings();
};

