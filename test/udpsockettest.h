#pragma once

#include "structs.h"
#include "widget.h"
#include "button.h"
#include "textedit.h"
#include "lineedit.h"
#include "label.h"
#include "udpsocket.h"

class UdpSocketTest : public SA::Widget
{
public:
    UdpSocketTest(SA::Widget *parent = nullptr);
    ~UdpSocketTest();

private:
    SA::TextEdit m_textEditRead;
    SA::TextEdit m_textEditSend;
    SA::LineEdit m_lineEditPortRead;
    SA::LineEdit m_lineEditPortSend;
    SA::LineEdit m_lineEditHostSend;
    SA::Label m_labelTextRead;
    SA::Label m_labelPortRead;
    SA::Label m_labelPortSend;
    SA::Label m_labelHostSend;
    SA::Label m_labelTextSend;
    SA::Button m_btnBind;
    SA::Button m_btnSend;
    SA::UdpSocket m_udpSocket;

    void btnBindPressed(bool state);
    void btnSendPressed(bool state);
    void dataReaded(const std::vector<char> &data);
    void resizeEvent(const SA::Size &size);
    void loadSettings();
    void saveSettings();
};

