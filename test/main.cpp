#include "application.h"
#include "snakegame.h"
#include "controls.h"
#include "udpsockettest.h"
#include "tcpsockettest.h"
#include "tcpservertest.h"

#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>

void signalHandler(int)
{
   SA::Application::instance().quit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

#ifdef __linux__
    signal(SIGKILL, signalHandler);
    signal(SIGQUIT, signalHandler);
#endif // Q_OS_WIN

   bool snake = false, controls = false;
   bool udpSocket = false, tcpSocket = false, tcpServer = false;

    if (argc > 1)
    {
        for (int i=1; i<argc; ++i)
        {
            if (strcmp(argv[i], "snake") == 0) snake = true;
            else if (strcmp(argv[i], "controls") == 0) controls = true;
            else if (strcmp(argv[i], "udp") == 0) udpSocket = true;
            else if (strcmp(argv[i], "tcp") == 0) tcpSocket = true;
            else if (strcmp(argv[i], "server") == 0) tcpServer = true;
        }
    }
    else
    {
        snake = true;
        controls = true;
        udpSocket = true;
        tcpSocket = true;
    }


    std::unique_ptr<SnakeGame> sg;
    if (snake)
    {
        sg = std::make_unique<SnakeGame>();
        sg->setGeometry(150, 150 , 410, 410);
        sg->show();
    }

    std::unique_ptr<Controls> ct;
    if (controls)
    {
        ct = std::make_unique<Controls>();
        ct->setGeometry(250, 180 , 410, 410);
        ct->show();
    }

    std::unique_ptr<UdpSocketTest> us;
    if (udpSocket)
    {
        us = std::make_unique<UdpSocketTest>();
        us->setGeometry(350, 210 , 610, 510);
        us->show();
    }

    std::unique_ptr<TcpSocketTest> ts;
    if (tcpSocket)
    {
        ts = std::make_unique<TcpSocketTest>();
        ts->setGeometry(450, 240 , 610, 510);
        ts->show();
    }

    std::unique_ptr<TcpServerTest>sr;
    if (tcpServer)
    {
        sr = std::make_unique<TcpServerTest>();
        sr->setGeometry(550, 270 , 610, 510);
        sr->show();
    }

    return SA::Application::instance().exec();
}
