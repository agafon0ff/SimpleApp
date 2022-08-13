#include "application.h"
#include "snakegame.h"
#include "controls.h"
#include "udpsockettest.h"

#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>

void sampleSnakeGame()
{
    SnakeGame sg;
    sg.setGeometry(150, 150 , 410, 410);
    sg.show();
}

void sampleControls()
{
    Controls tt;
    tt.setTitle("Controls");
    tt.setGeometry(565, 150 , 410, 410);
    tt.show();
}

void sampleUdpSocket()
{
    UdpSocketTest st;
    st.setTitle("UdpSocket");
    st.setGeometry(980, 150 , 610, 510);
    st.show();
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

   bool snake = false, controls = false, udpSocket = false;

    if (argc > 1)
    {
        for (int i=1; i<argc; ++i)
        {
            if (strcmp(argv[i], "snake") == 0) snake = true;
            else if (strcmp(argv[i], "controls") == 0) controls = true;
            else if (strcmp(argv[i], "udp") == 0) udpSocket = true;
        }
    }
    else
    {
        snake = true;
        controls = true;
        udpSocket = true;
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
        ct->setGeometry(565, 150 , 410, 410);
        ct->show();
    }

    std::unique_ptr<UdpSocketTest> us;
    if (udpSocket)
    {
        us = std::make_unique<UdpSocketTest>();
        us->setGeometry(980, 150 , 610, 510);
        us->show();
    }

    return SA::Application::instance().exec();
}
