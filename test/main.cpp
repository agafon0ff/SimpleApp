#include "application.h"
#include "snakegame.h"
#include "controls.h"
#include "udpsockettest.h"

#include <csignal>

void signalHandler(int)
{
   SA::Application::instance().quit(0);
}

int main()
{
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

#ifdef __linux__
    signal(SIGKILL, signalHandler);
    signal(SIGQUIT, signalHandler);
#endif // Q_OS_WIN

    SnakeGame sg;
    sg.setGeometry(150, 150 , 410, 410);
    sg.show();

    Controls tt;
    tt.setTitle("Controls");
    tt.setGeometry(565, 150 , 410, 410);
    tt.show();

    UdpSocketTest st;
    st.setTitle("UdpSocket");
    st.setGeometry(980, 150 , 610, 510);
    st.show();

    return SA::Application::instance().exec();
}
