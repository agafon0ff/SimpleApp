#include "application.h"
#include "snakegame.h"
#include "controls.h"
#include "udpsockettest.h"
#include "tcpsockettest.h"
#include "tcpservertest.h"
#include "imagetest.h"

#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

template <class T>
void createSample(std::vector<std::unique_ptr<SA::Widget>> &container, SA::Rect rect)
{
    container.emplace_back(std::make_unique<T>());
    container.back()->setGeometry(rect);
    container.back()->show();
}

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

   bool snake = false, controls = false, image = false;
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
            else if (strcmp(argv[i], "image") == 0) image = true;
        }
    }
    else
    {
        snake = true;
        controls = true;
        udpSocket = true;
        tcpSocket = true;
        tcpServer = true;
        image = true;
    }

    std::vector<std::unique_ptr<SA::Widget>> widgets;

    if (snake) createSample<SnakeGame>(widgets, {150, 150 , 410, 410});
    if (controls) createSample<Controls>(widgets, {250, 180 , 410, 410});
    if (udpSocket) createSample<UdpSocketTest>(widgets, {350, 210 , 610, 510});
    if (tcpSocket) createSample<TcpSocketTest>(widgets, {450, 240 , 610, 510});
    if (tcpServer) createSample<TcpServerTest>(widgets, {550, 270 , 610, 510});
    if (image) createSample<ImageTest>(widgets, {650, 300 , 300, 300});

    return SA::Application::instance().exec();
}
